/**
 * configurator.js
 *
 * Marlin Configuration Utility
 *    - Web form for entering configuration options
 *    - A reprap calculator to calculate movement values
 *    - Uses HTML5 to generate downloadables in Javascript
 *    - Reads and parses standard configuration files from local folders
 *
 * Supporting functions
 *    - Parser to read Marlin Configuration.h and Configuration_adv.h files
 *    - Utilities to replace values in configuration files
 */

"use strict";

$(function(){

var marlin_config = 'config';

// Extend String
String.prototype.lpad = function(len, chr) {
  if (chr === undefined) { chr = '&nbsp;'; }
  var s = this+'', need = len - s.length;
  if (need > 0) { s = new Array(need+1).join(chr) + s; }
  return s;
};
String.prototype.prePad = function(len, chr) { return len ? this.lpad(len, chr) : this; };
String.prototype.zeroPad = function(len)     { return this.prePad(len, '0'); };
String.prototype.toHTML = function()         { return jQuery('<div>').text(this).html(); };
String.prototype.regEsc = function()         { return this.replace(/[.?*+^$[\]\\(){}|-]/g, "\\$&"); }
String.prototype.lineCount = function()      { var len = this.split(/\r?\n|\r/).length; return len > 0 ? len - 1 : len; };

/**
 * selectField.addOptions takes an array or keyed object
 */
$.fn.extend({
  addOptions: function(arrObj) {
    return this.each(function() {
      var sel = $(this);
      var isArr = Object.prototype.toString.call(arrObj) == "[object Array]";
      $.each(arrObj, function(k, v) {
        sel.append( $('<option>',{value:isArr?v:k}).text(v) );
      });
    });
  }
});

// The app is a singleton
var configuratorApp = (function(){

  // private variables and functions go here
  var self,
      pi2 = Math.PI * 2,
      has_boards = false, has_config = false, has_config_adv = false,
      boards_file = 'boards.h',
      config_file = 'Configuration.h',
      config_adv_file = 'Configuration_adv.h',
      $tooltip = $('#tooltip'),
      $config = $('#config_text'),
      $config_adv = $('#config_adv_text'),
      boards_list = {},
      therms_list = {},
      total_config_lines,
      total_config_adv_lines,
      hover_timer,
      pulse_offset = 0;

  // Return this anonymous object as configuratorApp
  return {
    my_public_var: 4,
    logging: 1,

    init: function() {
      self = this; // a 'this' for use when 'this' is something else

      // Set up the form
      this.initConfigForm();

      // Make tabs for the fieldsets
      var $fset = $('#config_form fieldset');
      var $tabs = $('<ul>',{class:'tabs'}), ind = 1;
      $('#config_form fieldset').each(function(){
        var tabID = 'TAB'+ind;
        $(this).addClass(tabID);
        var $leg = $(this).find('legend');
        var $link = $('<a>',{href:'#'+ind,id:tabID}).text($leg.text());
        $tabs.append($('<li>').append($link));
        $link.click(function(e){
          e.preventDefault;
          var ind = this.id;
          $tabs.find('.active').removeClass('active');
          $(this).addClass('active');
          $fset.hide();
          $fset.filter('.'+this.id).show();
          return false;
        });
        ind++;
      });
      $tabs.appendTo('#tabs');
      $('<br>',{class:'clear'}).appendTo('#tabs');
      $tabs.find('a:first').trigger('click');

      // Make a droppable file uploader, if possible
      var $uploader = $('#file-upload');
      var fileUploader = new BinaryFileUploader({
        element:    $uploader[0],
        onFileLoad: function(file) { self.handleFileLoad(file, $uploader); }
      });
      if (!fileUploader.hasFileUploaderSupport())
        this.setMessage("Your browser doesn't support the file reading API.", 'error');

      // Read boards.h, Configuration.h, Configuration_adv.h
      var ajax_count = 0, success_count = 0;
      var loaded_items = {};
      var config_files = [boards_file, config_file, config_adv_file];
      $.each(config_files, function(i,fname){
        $.ajax({
          url: marlin_config+'/'+fname,
          type: 'GET',
          async: true,
          cache: false,
          success: function(txt) {
            loaded_items[fname] = function(){ self.fileLoaded(fname, txt); };
            success_count++;
          },
          complete: function() {
            ajax_count++;
            if (ajax_count >= 3) {
              $.each(config_files, function(i,fname){ if (loaded_items[fname] !== undefined) loaded_items[fname](); });
              self.refreshConfigForm();
              if (success_count < ajax_count)
                self.setMessage('Unable to load configurations. Use the upload field instead.', 'error');
            }
          }
        });
      });
    },

    setMessage: function(msg,type) {
      if (msg) {
        if (type === undefined) type = 'message';
        var $err = $('<p class="'+type+'">'+msg+'</p>'), err = $err[0];
        $('#message').prepend($err);
        var baseColor = $err.css('color').replace(/rgba?\(([^),]+,[^),]+,[^),]+).*/, 'rgba($1,');
        var d = new Date();
        err.pulse_offset = (pulse_offset += 200);
        err.startTime = d.getTime() + pulse_offset;
        err.pulser = setInterval(function(){
            d = new Date();
            var pulse_time = d.getTime() + err.pulse_offset;
            $err.css({color:baseColor+(0.5+Math.sin(pulse_time/200)*0.4)+')'});
            if (pulse_time - err.startTime > 5000) {
              clearInterval(err.pulser);
              $err.remove();
            }
          }, 50);
      }
      else {
        $('#message p.error, #message p.warning').each(function() {
          if (this.pulser !== undefined && this.pulser)
            clearInterval(this.pulser);
          $(this).remove();
        });
      }
    },

    /**
     * Init the boards array from a boards.h file
     */
    initBoardsFromText: function(txt) {
      boards_list = {};
      var r, findDef = new RegExp('[ \\t]*#define[ \\t]+(BOARD_[\\w_]+)[ \\t]+(\\d+)[ \\t]*(//[ \\t]*)?(.+)?', 'gm');
      while((r = findDef.exec(txt)) !== null) {
        boards_list[r[1]] = r[2].prePad(3, '  ') + " — " + r[4].replace(/\).*/, ')');
      }
      this.log("Loaded boards", 3); this.log(boards_list, 3);
      has_boards = true;
    },

    /**
     * Init the thermistors array from the Configuration.h file
     */
    initThermistorsFromText: function(txt) {
      // Get all the thermistors and save them into an object
      var r, s, findDef = new RegExp('(//.*\n)+\\s+(#define[ \\t]+TEMP_SENSOR_0)', 'g');
      r = findDef.exec(txt);
      findDef = new RegExp('^//[ \\t]*([-\\d]+)[ \\t]+is[ \\t]+(.*)[ \\t]*$', 'gm');
      while((s = findDef.exec(r[0])) !== null) {
        therms_list[s[1]] = s[1].prePad(4, '  ') + " — " + s[2];
      }
    },

    /**
     * Handle a file being dropped on the file field
     */
    handleFileLoad: function(txt, $uploader) {
      txt += '';
      var filename = $uploader.val().replace(/.*[\/\\](.*)$/, '$1');
      switch(filename) {
        case boards_file:
        case config_file:
        case config_adv_file:
          this.fileLoaded(filename, txt);
          break;
        default:
          this.log("Can't parse "+filename, 1);
          break;
      }
    },

    /**
     * Process a file after it's been successfully loaded
     */
    fileLoaded: function(filename, txt) {
      this.log("fileLoaded:"+filename,4);
      switch(filename) {
        case boards_file:
          this.initBoardsFromText(txt);
          $('#MOTHERBOARD').html('').addOptions(boards_list);
          if (has_config) this.initField('MOTHERBOARD');
          this.setMessage(boards_file+' loaded successfully.');
          break;
        case config_file:
          if (has_boards) {
            $config.text(txt);
            total_config_lines = txt.lineCount();
            this.initThermistorsFromText(txt);
            this.purgeDefineInfo(false);
            this.refreshConfigForm();
            this.setMessage(config_file+' loaded successfully.');
            has_config = true;
          }
          else {
            this.setMessage("Upload a " + boards_file + " file first!", 'error');
          }
          break;
        case config_adv_file:
          if (has_config) {
            $config_adv.text(txt);
            total_config_adv_lines = txt.lineCount();
            this.purgeDefineInfo(true);
            this.refreshConfigForm();
            this.setMessage(config_adv_file+' loaded successfully.');
            has_config_adv = true;
          }
          else {
            this.setMessage("Upload a " + config_file + " file first!", 'error');
          }
          break;
      }
    },

    /**
     * Add enhancements to the form
     */
    initConfigForm: function() {
      // Modify form fields and make the form responsive.
      // As values change on the form, we could update the
      // contents of text areas containing the configs, for
      // example.

      // while(!$config_adv.text() == null) {}
      // while(!$config.text() == null) {}

      // Go through all form items with names
      $('#config_form').find('[name]').each(function() {
        // Set its id to its name
        var name = $(this).attr('name');
        $(this).attr({id: name});
        // Attach its label sibling
        var $label = $(this).prev('label');
        if ($label.length) $label.attr('for',name);
      });

      // Get all 'switchable' class items and add a checkbox
      $('#config_form .switchable').each(function(){
        $(this).after(
          $('<input>',{type:'checkbox',value:'1',class:'enabler'}).prop('checked',true)
            .attr('id',this.id + '-switch')
            .change(self.handleSwitch)
        );
      });

      // Add options to the popup menus
      $('#SERIAL_PORT').addOptions([0,1,2,3,4,5,6,7]);
      $('#BAUDRATE').addOptions([2400,9600,19200,38400,57600,115200,250000]);
      $('#EXTRUDERS').addOptions([1,2,3,4]);
      $('#POWER_SUPPLY').addOptions({'1':'ATX','2':'Xbox 360'});

      // Replace the Serial popup menu with a stepper control
      $('#serial_stepper').jstepper({
        min: 0,
        max: 3,
        val: $('#SERIAL_PORT').val(),
        arrowWidth: '18px',
        arrowHeight: '15px',
        color: '#FFF',
        acolor: '#F70',
        hcolor: '#FF0',
        id: 'select-me',
        textStyle: {width:'1.5em',fontSize:'120%',textAlign:'center'},
        onChange: function(v) { $('#SERIAL_PORT').val(v).trigger('change'); }
      });
    },

    /**
     * Update all fields on the form after loading a configuration
     */
    refreshConfigForm: function() {

      /**
       * For now I'm manually creating these references
       * but I should be able to parse Configuration.h
       * and iterate the #defines.
       *
       * For any #ifdef blocks I can create field groups
       * which can be dimmed together when the option
       * is disabled.
       *
       * Then we only need to specify exceptions to
       * standard behavior, (which is to add a text field)
       */
      this.initField('SERIAL_PORT');

      this.initField('BAUDRATE');

      this.initField('BTENABLED');

      $('#MOTHERBOARD').html('').addOptions(boards_list);
      this.initField('MOTHERBOARD');

      this.initField('CUSTOM_MENDEL_NAME');

      this.initField('MACHINE_UUID');

      this.initField('EXTRUDERS');

      this.initField('POWER_SUPPLY');

      this.initField('PS_DEFAULT_OFF');

      $('#TEMP_SENSOR_0, #TEMP_SENSOR_1, #TEMP_SENSOR_2, #TEMP_SENSOR_BED').html('').addOptions(therms_list);
      this.initField('TEMP_SENSOR_0');
      this.initField('TEMP_SENSOR_1');
      this.initField('TEMP_SENSOR_2');
      this.initField('TEMP_SENSOR_BED');

      this.initField('TEMP_SENSOR_1_AS_REDUNDANT');
      this.initField('MAX_REDUNDANT_TEMP_SENSOR_DIFF');

      this.initField('TEMP_RESIDENCY_TIME');
    },

    /**
     * Make a field responsive and initialize its defineInfo
     */
    initField: function(name, adv) {
      this.log("initField:"+name,4);
      var $elm = $('#'+name), elm = $elm[0];
      if (elm.defineInfo == null) {
        var inf = elm.defineInfo = this.getDefineInfo(name, adv);
        $elm.on($elm.attr('type') == 'text' ? 'input' : 'change', this.handleChange);

        if (inf.comment) {
          var $tipme = $elm.prev('label');
          if ($tipme.length) {
            $tipme.hover(
              function() {
                var pos = $tipme.position();
                $tooltip.html(inf.comment)
                  .append('<span>')
                  .css({bottom:($tooltip.parent().outerHeight()-pos.top)+'px',left:(pos.left+70)+'px'})
                  .show();
                if (hover_timer) {
                  clearTimeout(hover_timer);
                  hover_timer = null;
                }
              },
              function() {
                hover_timer = setTimeout(function(){
                  hover_timer = null;
                  $tooltip.fadeOut(400);
                }, 400);
              }
            );
          }
        }
      }
      this.setFieldFromDefine(name);
    },

    /**
     * Handle any value field being changed
     */
    handleChange: function() { self.updateDefineFromField(this.id); },

    /**
     * Handle a switch checkbox being changed
     */
    handleSwitch: function() {
      var $elm = $(this), $prev = $elm.prev();
      var on = $elm.prop('checked') || false;
      $prev.attr('disabled', !on);
      self.setDefineEnabled($prev[0].id, on);
    },

    /**
     * Get the current value of a #define (from the config text)
     */
    defineValue: function(name) {
      this.log('defineValue:'+name,4);
      var $elm = $('#'+name), elm = $elm[0], inf = elm.defineInfo;
      if (inf == null) return 'n/a';
      var result = inf.regex.exec($(inf.field).text());

      this.log(result,2);

      return inf.type == 'switch' ? result[inf.val_i] != '//' : result[inf.val_i];
    },

    /**
     * Get the current enabled state of a #define (from the config text)
     */
    defineIsEnabled: function(name) {
      this.log('defineIsEnabled:'+name,4);
      var $elm = $('#'+name), elm = $elm[0], inf = elm.defineInfo;
      if (inf == null) return false;
      var result = inf.regex.exec($(inf.field).text());

      this.log(result,2);

      var on = result !== null ? result[1].trim() != '//' : true;
      this.log(name + ' = ' + on, 2);

      return on;
    },

    /**
     * Set a #define enabled or disabled by altering the config text
     */
    setDefineEnabled: function(name, val) {
      this.log('setDefineEnabled:'+name,4);
      var $elm = $('#'+name), inf = $elm[0].defineInfo;
      if (inf == null) return;

      var slash = val ? '' : '//';
      var newline = inf.line
        .replace(/^([ \t]*)(\/\/)([ \t]*)/, '$1$3')              // remove slashes
        .replace(inf.pre+inf.define, inf.pre+slash+inf.define);  // add them back

      this.setDefineLine(name, newline);
    },

    /**
     * Update a #define (from the form) by altering the config text
     */
    updateDefineFromField: function(name) {
      this.log('updateDefineFromField:'+name,4);
      var $elm = $('#'+name), inf = $elm[0].defineInfo;
      if (inf == null) return;

      var isCheck = $elm.attr('type') == 'checkbox',
          val = isCheck ? $elm.prop('checked') : $elm.val();

      var newline;
      switch(inf.type) {
        case 'switch':
          var slash = val ? '' : '//';
          newline = inf.line.replace(inf.repl, '$1'+slash+'$3');
          break;
        case 'quoted':
        case 'plain':
          if (isCheck)
            this.setMessage(name + ' should not be a checkbox!', 'error');
          else
            newline = inf.line.replace(inf.repl, '$1'+val.replace('$','\\$')+'$3');
          break;
      }
      this.setDefineLine(name, newline);
    },

    /**
     * Set the define's line in the text to a new line,
     *   then update, highlight, and scroll to the line
     */
    setDefineLine: function(name, newline) {
      this.log('setDefineLine:'+name+'\n'+newline,4);
      var $elm = $('#'+name), elm = $elm[0], inf = elm.defineInfo;
      var $c = $(inf.field), txt = $c.text();

      var hilite_token = '[HIGHLIGHTER-TOKEN]';

      txt = txt.replace(inf.line, hilite_token + newline);
      inf.line = newline;

      // Convert txt into HTML before storing
      var html = txt.toHTML().replace(hilite_token, '<span></span>');

      // Set the final text including the highlighter
      $c.html(html);

      // Scroll to reveal the define
      this.scrollToDefine(name);
    },

    /**
     * Scroll a pre box to reveal a #define
     */
    scrollToDefine: function(name, always) {
      this.log('scrollToDefine:'+name,4);
      var $elm = $('#'+name), inf = $elm[0].defineInfo, $c = $(inf.field);

      // Scroll to the altered text if it isn't visible
      var halfHeight = $c.height()/2, scrollHeight = $c.prop('scrollHeight'),
          textScrollY = inf.lineNum * scrollHeight/(inf.adv ? total_config_adv_lines : total_config_lines) - halfHeight;

      if (textScrollY < 0)
        textScrollY = 0;
      else if (textScrollY > scrollHeight)
        textScrollY = scrollHeight - 1;

      if (always == true || Math.abs($c.prop('scrollTop') - textScrollY) > halfHeight)
        $c.animate({ scrollTop: textScrollY < 0 ? 0 : textScrollY });
    },

    /**
     * Set a form field to the current #define value in the config text
     */
    setFieldFromDefine: function(name) {
      var $elm = $('#'+name), val = this.defineValue(name);

      this.log('setFieldFromDefine:' + name + ' to ' + val, 4);

      // Set the field value
      $elm.attr('type') == 'checkbox' ? $elm.prop('checked', val) : $elm.val(''+val);

      // If the item has a checkbox then set enabled state too
      var $cb = $('#'+name+'-switch');
      if ($cb.length) {
        var on = self.defineIsEnabled(name);
        $elm.attr('disabled', !on); // enable/disable the form field (could also dim it)
        $cb.prop('checked', on);    // check/uncheck the checkbox
      }
    },

    /**
     * Purge #define information for one of the config files
     */
    purgeDefineInfo: function(adv) {
      if (adv === undefined) adv = false;
      $('[name]').each(function() {
        var inf = this.defineInfo;
        if (inf && adv === inf.adv) $(this).removeProp('defineInfo');
      });
    },

    /**
     * Update #define information for one of the config files
     */
    refreshDefineInfo: function(adv) {
      if (adv === undefined) adv = false;
      $('[name]').each(function() {
        var inf = this.defineInfo;
        if (inf && adv == inf.adv) this.defineInfo = self.getDefineInfo(this.id, adv);
      });
    },

    /**
     * Get information about a #define from configuration file text:
     *
     *   Pre-examine the #define for its prefix, value position, suffix, etc.
     *   Construct a regex for the #define to quickly find (and replace) values.
     *   Store the existing #define line as the key to finding it later.
     *   Determine the line number of the #define so it can be scrolled to.
     */
    getDefineInfo: function(name, adv) {
      if (adv === undefined) adv = false;
      this.log('getDefineInfo:'+name,4);
      var $elm = $('#'+name), elm = $elm[0],
          $c = adv ? $config_adv : $config,
          txt = $c.text();

      // a switch line with no value
      var findDef = new RegExp('^([ \\t]*//)?([ \\t]*#define[ \\t]+' + elm.id + ')([ \\t]*/[*/].*)?$', 'm'),
          result = findDef.exec(txt),
          info = { type:0, adv:adv, field:$c[0], val_i: 2 };
      if (result !== null) {
        $.extend(info, {
          val_i:  1,
          type:   'switch',
          line:   result[0], // whole line
          pre:    result[1] === undefined ? '' : result[1].replace('//',''),
          define: result[2],
          post:   result[3] === undefined ? '' : result[3]
        });
        info.regex = new RegExp('([ \\t]*//)?([ \\t]*' + info.define.regEsc() + info.post.regEsc() + ')', 'm');
        info.repl =  new RegExp('([ \\t]*)(\/\/)?([ \\t]*' + info.define.regEsc() + info.post.regEsc() + ')', 'm');
      }
      else {
        // a define with quotes
        findDef = new RegExp('^(.*//)?(.*#define[ \\t]+' + elm.id + '[ \\t]+)("[^"]*")([ \\t]*/[*/].*)?$', 'm');
        result = findDef.exec(txt);
        if (result !== null) {
          $.extend(info, {
            type:   'quoted',
            line:   result[0],
            pre:    result[1] === undefined ? '' : result[1].replace('//',''),
            define: result[2],
            post:   result[4] === undefined ? '' : result[4]
          });
          info.regex = new RegExp('([ \\t]*//)?[ \\t]*' + info.define.regEsc() + '"([^"]*)"' + info.post.regEsc(), 'm');
          info.repl  = new RegExp('(([ \\t]*//)?[ \\t]*' + info.define.regEsc() + '")[^"]*("' + info.post.regEsc() + ')', 'm');
        }
        else {
          // a define with no quotes
          findDef = new RegExp('^([ \\t]*//)?([ \\t]*#define[ \\t]+' + elm.id + '[ \\t]+)(\\S*)([ \\t]*/[*/].*)?$', 'm');
          result = findDef.exec(txt);
          if (result !== null) {
            $.extend(info, {
              type:   'plain',
              line:   result[0],
              pre:    result[1] === undefined ? '' : result[1].replace('//',''),
              define: result[2],
              post:   result[4] === undefined ? '' : result[4]
            });
            info.regex = new RegExp('([ \\t]*//)?[ \\t]*' + info.define.regEsc() + '(\\S*)' + info.post.regEsc(), 'm');
            info.repl  = new RegExp('(([ \\t]*//)?[ \\t]*' + info.define.regEsc() + ')\\S*(' + info.post.regEsc() + ')', 'm');
          }
        }
      }

      if (info.type) {
        // Get the end-of-line comment, if there is one
        var comment = '';
        findDef = new RegExp('.*#define[ \\t].*/[/*]+[ \\t]*(.*)');
        if (info.line.search(findDef) >= 0)
          comment = info.line.replace(findDef, '$1');

        // Get all the comments immediately before the item
        var r, s;
        findDef = new RegExp('(([ \\t]*(//|#)[^\n]+\n){1,4})([ \\t]*\n){0,1}' + info.line.regEsc(), 'g');
        if (r = findDef.exec(txt)) {
          findDef = new RegExp('^[ \\t]*//+[ \\t]*(.*)[ \\t]*$', 'gm');
          while((s = findDef.exec(r[1])) !== null) {
            if (s[1].match(/^#define[ \\t]/) != null) {
              comment = '';
              break;
            }
            comment += ' ' + s[1] + "\n";
          }
        }

        findDef = new RegExp('^[ \\t]*'+name+'[ \\t]*', 'm');
        $.extend(info, {
          comment: '<strong>'+name+'</strong> '+comment.replace(findDef,'').trim().toHTML(),
          lineNum: this.getLineNumberOfText(info.line, txt)
        });
      }
      else
        info = null;

      this.log(info,2);

      return info;
    },

    /**
     * Count the number of lines before a match, return -1 on fail
     */
    getLineNumberOfText: function(line, txt) {
      var pos = txt.indexOf(line);
      return (pos < 0) ? pos : txt.substr(0, pos).lineCount();
    },

    log: function(o,l) {
      if (l === undefined) l = 0;
      if (this.logging>=l*1) console.log(o);
    },

    logOnce: function(o) {
      if (o.didLogThisObject === undefined) {
        this.log(o);
        o.didLogThisObject = true;
      }
    },

    EOF: null
  };

})();

// Typically the app would be in its own file, but this would be here
configuratorApp.init();

});
