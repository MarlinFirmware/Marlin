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

/**
 * Github API useful GET paths. (Start with "https://api.github.com/repos/:owner/:repo/")
 *
 *   contributors                               Get a list of contributors
 *   tags                                       Get a list of tags
 *   contents/[path]?ref=branch/tag/commit      Get the contents of a file
 */

 // GitHub
 // Warning! Limited to 60 requests per hour!
var config = {
  type:  'github',
  host:  'https://api.github.com',
  owner: 'thinkyhead',
  repo:  'Marlin',
  ref:   'marlin_configurator',
  path:  'Marlin/configurator/config'
};
/**/

/* // Remote
var config = {
  type:  'remote',
  host:  'http://www.thinkyhead.com',
  path:  '_marlin/config'
};
/**/

/* // Local
var config = {
  type:  'local',
  path:  'config'
};
/**/

function github_command(conf, command, path) {
  var req = conf.host+'/repos/'+conf.owner+'/'+conf.repo+'/'+command;
  if (path) req += '/' + path;
  return req;
}
function config_path(item) {
  var path = '', ref = '';
  switch(config.type) {
    case 'github':
      path = github_command(config, 'contents', config.path);
      if (config.ref !== undefined) ref = '?ref=' + config.ref;
      break;
    case 'remote':
      path = config.host + '/' + config.path + '/';
      break;
    case 'local':
      path = config.path + '/';
      break;
  }
  return path + '/' + item + ref;
}

// Extend builtins
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
String.prototype.lineCount = function()      { var len = this.split(/\r?\n|\r/).length; return len > 0 ? len - 1 : 0; };
String.prototype.toLabel = function()        { return this.replace(/_/g, ' ').toTitleCase(); }
String.prototype.toTitleCase = function()    { return this.replace(/([A-Z])(\w+)/gi, function(m,p1,p2) { return p1.toUpperCase() + p2.toLowerCase(); }); }
Number.prototype.limit = function(m1, m2)  {
  if (m2 == null) return this > m1 ? m1 : this;
  return this < m1 ? m1 : this > m2 ? m2 : this;
};
Date.prototype.fileStamp = function(filename) {
  var fs = this.getFullYear()
    + ((this.getMonth()+1)+'').zeroPad(2)
    + (this.getDate()+'').zeroPad(2)
    + (this.getHours()+'').zeroPad(2)
    + (this.getMinutes()+'').zeroPad(2)
    + (this.getSeconds()+'').zeroPad(2);

  if (filename !== undefined)
    return filename.replace(/^(.+)(\.\w+)$/g, '$1-['+fs+']$2');

  return fs;
}

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
  },
  noSelect: function() {
    return this
            .attr('unselectable', 'on')
            .css('user-select', 'none')
            .on('selectstart', false);
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
      $msgbox = $('#message'),
      $form = $('#config_form'),
      $tooltip = $('#tooltip'),
      $config = $('#config_text pre'),
      $config_adv = $('#config_adv_text pre'),
      define_list = [[],[]],
      define_section = {},
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

      // Set up the form, creating fields and fieldsets as-needed
      this.initConfigForm();

      // Make tabs for all the fieldsets
      this.makeTabsForFieldsets();

      // No selection on errors
      // $msgbox.noSelect();

      // Make a droppable file uploader, if possible
      var $uploader = $('#file-upload');
      var fileUploader = new BinaryFileUploader({
        element:    $uploader[0],
        onFileLoad: function(file) { self.handleFileLoad(file, $uploader); }
      });
      if (!fileUploader.hasFileUploaderSupport())
        this.setMessage("Your browser doesn't support the file reading API.", 'error');

      // Make the disclosure items work
      $('.disclose').click(function(){
        var $dis = $(this), $pre = $dis.nextAll('pre:first');
        var didAnim = function() {$dis.toggleClass('closed almost');};
        $dis.addClass('almost').hasClass('closed')
          ? $pre.slideDown(200, didAnim)
          : $pre.slideUp(200, didAnim);
      });

      // Read boards.h, Configuration.h, Configuration_adv.h
      var ajax_count = 0, success_count = 0;
      var loaded_items = {};
      var config_files = [boards_file, config_file, config_adv_file];
      var isGithub = config.type == 'github';
      var rateLimit = 0;
      $.each(config_files, function(i,fname){
        var url = config_path(fname);
        $.ajax({
          url: url,
          type: 'GET',
          dataType: isGithub ? 'jsonp' : undefined,
          async: true,
          cache: false,
          error: function(req, stat, err) {
            self.log(req, 1);
            if (req.status == 200) {
              if (typeof req.responseText === 'string') {
                var txt = req.responseText;
                loaded_items[fname] = function(){ self.fileLoaded(fname, txt); };
                success_count++;
                // self.setMessage('The request for "'+fname+'" may be malformed.', 'error');
              }
            }
            else {
              self.setRequestError(req.status ? req.status : '(Access-Control-Allow-Origin?)', url);
            }
          },
          success: function(txt) {
            if (isGithub && typeof txt.meta.status !== undefined && txt.meta.status != 200) {
              self.setRequestError(txt.meta.status, url);
            }
            else {
              // self.log(txt, 1);
              if (isGithub) {
                rateLimit = {
                  quota: 1 * txt.meta['X-RateLimit-Remaining'],
                  timeLeft: Math.floor(txt.meta['X-RateLimit-Reset'] - Date.now()/1000),
                };
              }
              loaded_items[fname] = function(){ self.fileLoaded(fname, isGithub ? atob(txt.data.content) : txt); };
              success_count++;
            }
          },
          complete: function() {
            ajax_count++;
            if (ajax_count >= config_files.length) {
              // If not all files loaded set an error
              if (success_count < ajax_count)
                self.setMessage('Unable to load configurations. Try the upload field.', 'error');

              // Is the request near the rate limit? Set an error.
              var r;
              if (r = rateLimit) {
                if (r.quota < 20) {
                  self.setMessage(
                    'Approaching request limit (' +
                    r.quota + ' remaining.' +
                    ' Reset in ' + Math.floor(r.timeLeft/60) + ':' + (r.timeLeft%60+'').zeroPad(2) + ')',
                    'warning'
                  );
                }
              }
              // Post-process all the loaded files
              $.each(config_files, function(){ if (loaded_items[this]) loaded_items[this](); });
            }
          }
        });
      });
    },

    /**
     * Make a download link visible and active
     */
    activateDownloadLink: function(adv) {
      var $c = adv ? $config_adv : $config, txt = $c.text();
      var filename = (adv ? config_adv_file : config_file);
      $c.prevAll('.download:first')
        .unbind('mouseover click')
        .mouseover(function() {
          var d = new Date(), fn = d.fileStamp(filename);
          $(this).attr({ download:fn, href:'download:'+fn, title:'download:'+fn });
        })
        .click(function(){
          var $button = $(this);
          $(this).attr({ href:'data:text/plain;charset=utf-8,' + encodeURIComponent($c.text()) });
          setTimeout(function(){
            $button.attr({ href:$button.attr('title') });
          }, 100);
          return true;
        })
        .css({visibility:'visible'});
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
      this.log("Loaded boards:\n" + Object.keys(boards_list).join('\n'), 3);
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
     * Get all the unique define names
     */
    updateDefinesFromText: function(index, txt) {
      var section = 'machine',
          leave_out_defines = ['CONFIGURATION_H', 'CONFIGURATION_ADV_H', 'STRING_VERSION', 'STRING_URL', 'STRING_VERSION_CONFIG_H', 'STRING_CONFIG_H_AUTHOR', 'STRING_SPLASH_LINE1', 'STRING_SPLASH_LINE2'],
          define_sect = {},
          r, findDef = new RegExp('(@section|#define)[ \\t]+(\\w+)', 'gm');
      while((r = findDef.exec(txt)) !== null) {
        var name = r[2];
        if (r[1] == '@section')
          section = name;
        else if ($.inArray(name, leave_out_defines) < 0 && !(name in define_sect))
          define_sect[name] = section;
      }
      define_list[index] = Object.keys(define_sect);
      $.extend(define_section, define_sect);
      this.log(define_list[index], 2);
    },

    /**
     * Create fields for any defines that have none
     */
    createFieldsForDefines: function(adv) {
      var e = adv ? 1 : 0, n = 0;
      var fail_list = [];
      $.each(define_list[e], function(i,name) {
        var section = define_section[name];
        if (section != 'hidden' && !$('#'+name).length) {
          var inf = self.getDefineInfo(name, adv);

          if (inf) {

            var $ff = $('#'+section), $newfield,
                $newlabel = $('<label>',{for:name,class:'added'}).text(name.toLabel());

            // if (!(++n % 3))
              $newlabel.addClass('newline');

            $ff.append($newlabel);

            // Multiple fields?
            if (inf.type == 'list') {
              for (var i=0; i<inf.size; i++) {
                var fieldname = i > 0 ? name+'-'+i : name;
                $newfield = $('<input>',{type:'text',size:6,maxlength:10,id:fieldname,name:fieldname,class:'subitem added'}).prop({defineInfo:inf});
                $ff.append($newfield);
              }
            }
            else {
              // Items with options, either toggle or select
              // TODO: Radio buttons for other values
              if (inf.options !== undefined) {
                if (inf.type == 'toggle') {
                  $newfield = $('<input>',{type:'checkbox'});
                }
                else {
                  // Otherwise selectable
                  $newfield = $('<select>');
                }
                // ...Options added when field initialized
              }
              else {
                $newfield = inf.type == 'switch' ? $('<input>',{type:'checkbox'}) : $('<input>',{type:'text',size:10,maxlength:40});
              }
              $newfield.attr({id:name,name:name,class:'added'}).prop({defineInfo:inf});
              // Add the new field to the form
              $ff.append($newfield);
            }
          }
          else
            fail_list.push(name);
        }
      });
      if (fail_list) this.log('Unable to parse:\n' + fail_list.join('\n'), 2);
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
          this.setMessage("Can't parse '"+filename+"'!");
          break;
      }
    },

    /**
     * Process a file after it's been successfully loaded
     */
    fileLoaded: function(filename, txt) {
      this.log("fileLoaded:"+filename,4);
      var err, init_index;
      switch(filename) {
        case boards_file:
          this.initBoardsFromText(txt);
          $('#MOTHERBOARD').html('').addOptions(boards_list);
          if (has_config) this.initField('MOTHERBOARD');
          break;
        case config_file:
          if (has_boards) {
            $config.text(txt);
            total_config_lines = txt.lineCount();
            // this.initThermistorsFromText(txt);
            init_index = 0;
            has_config = true;
          }
          else {
            err = boards_file;
          }
          break;
        case config_adv_file:
          if (has_config) {
            $config_adv.text(txt);
            total_config_adv_lines = txt.lineCount();
            init_index = 1;
            has_config_adv = true;
          }
          else {
            err = config_file;
          }
          break;
      }
      // When a config file loads defines might change
      if (init_index != null) {
        var adv = init_index == 1;
        this.purgeAddedFields(init_index);
        this.updateDefinesFromText(init_index, txt);
        this.createFieldsForDefines(adv);
        this.refreshConfigForm(init_index);
        this.activateDownloadLink(adv);
      }
      this.setMessage(err
        ? 'Please upload a "' + boards_file + '" file first!'
        : '"' + filename + '" loaded successfully.', err ? 'error' : 'message'
      );
    },

    /**
     * Add initial enhancements to the existing form
     */
    initConfigForm: function() {
      // Modify form fields and make the form responsive.
      // As values change on the form, we could update the
      // contents of text areas containing the configs, for
      // example.

      // while(!$config_adv.text() == null) {}
      // while(!$config.text() == null) {}

      // Go through all form items with names
      $form.find('[name]').each(function() {
        // Set its id to its name
        var name = $(this).attr('name');
        $(this).attr({id: name});
        // Attach its label sibling
        var $label = $(this).prev('label');
        if ($label.length) $label.attr('for',name);
      });

      // Get all 'switchable' class items and add a checkbox
      // $form.find('.switchable').each(function(){
      //   $(this).after(
      //     $('<input>',{type:'checkbox',value:'1',class:'enabler added'})
      //       .prop('checked',true)
      //       .attr('id',this.id + '-switch')
      //       .change(self.handleSwitch)
      //   );
      // });

      // Add options to the popup menus
      // $('#SERIAL_PORT').addOptions([0,1,2,3,4,5,6,7]);
      // $('#BAUDRATE').addOptions([2400,9600,19200,38400,57600,115200,250000]);
      // $('#EXTRUDERS').addOptions([1,2,3,4]);
      // $('#POWER_SUPPLY').addOptions({'1':'ATX','2':'Xbox 360'});

      // Replace the Serial popup menu with a stepper control
      /*
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
      */
    },

    /**
     * Make tabs to switch between fieldsets
     */
    makeTabsForFieldsets: function() {
      // Make tabs for the fieldsets
      var $fset = $form.find('fieldset'),
          $tabs = $('<ul>',{class:'tabs'}),
          ind = 1;
      $fset.each(function(){
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
      $('#tabs').html('').append($tabs);
      $('<br>',{class:'clear'}).appendTo('#tabs');
      $tabs.find('a:first').trigger('click');
    },

    /**
     * Update all fields on the form after loading a configuration
     */
    refreshConfigForm: function(init_index) {

      /**
       * Any manually-created form elements will remain
       * where they are. Unknown defines (currently most)
       * are added to tabs based on section
       *
       * Specific exceptions can be managed by applying
       * classes to the associated form fields.
       * Sorting and arrangement can come from an included
       * Javascript file that describes the configuration
       * in JSON, or using information added to the config
       * files.
       *
       */

      // Refresh the motherboard menu with new options
      $('#MOTHERBOARD').html('').addOptions(boards_list);

      // Init all existing fields, getting define info for any that need it
      // refreshing the options and updating their current values
      $.each(define_list[init_index], function() {
        if ($('#'+this).length)
          self.initField(this,init_index==1);
        else
          self.log(this + " is not on the page yet.", 2);
      });
    },

    /**
     * Get the defineInfo for a field on the form
     * Make it responsive, add a tooltip
     */
    initField: function(name, adv) {
      this.log("initField:"+name,4);
      var $elm = $('#'+name), elm = $elm[0], inf = elm.defineInfo;
      if (inf == null)
        inf = elm.defineInfo = this.getDefineInfo(name, adv);

      // Create a tooltip on the label if there is one
      if (inf.tooltip) {
        var $tipme = $elm.prev('label');
        if ($tipme.length) {
          $tipme.unbind('mouseenter mouseleave');
          $tipme.hover(
            function() {
              if ($('#tipson input').prop('checked')) {
                var pos = $tipme.position();
                $tooltip.html(inf.tooltip)
                  .append('<span>')
                  .css({bottom:($tooltip.parent().outerHeight()-pos.top)+'px',left:(pos.left+70)+'px'})
                  .show();
                if (hover_timer) {
                  clearTimeout(hover_timer);
                  hover_timer = null;
                }
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

      // Make the element(s) respond to events
      if (inf.type == 'list') {
        // Multiple fields need to respond
        for (var i=0; i<inf.size; i++) {
          if (i > 0) $elm = $('#'+name+'-'+i);
          $elm.unbind('input');
          $elm.on('input', this.handleChange);
        }
      }
      else {
        var elmtype = $elm.attr('type');
        // Set options on single fields if there are any
        if (inf.options !== undefined && elmtype === undefined)
          $elm.html('').addOptions(inf.options);
        $elm.unbind('input change');
        $elm.on(elmtype == 'text' ? 'input' : 'change', this.handleChange);
      }

      // Add an enabler checkbox if it needs one
      if (inf.switchable && $('#'+name+'-switch').length == 0) {
        // $elm = the last element added
        $elm.after(
          $('<input>',{type:'checkbox',value:'1',class:'enabler added'})
            .prop('checked',self.defineIsEnabled(name))
            .attr({id: name+'-switch'})
            .change(self.handleSwitch)
        );
      }

      // Set the field's initial value from the define
      this.setFieldFromDefine(name);
    },

    /**
     * Handle any value field being changed
     * this = the field
     */
    handleChange: function() { self.updateDefineFromField(this.id); },

    /**
     * Handle a switch checkbox being changed
     * this = the switch checkbox
     */
    handleSwitch: function() {
      var $elm = $(this),
          name = $elm[0].id.replace(/-.+/,''),
          inf = $('#'+name)[0].defineInfo,
          on = $elm.prop('checked') || false;

      self.setDefineEnabled(name, on);

      if (inf.type == 'list') {
        // Multiple fields?
        for (var i=0; i<inf.size; i++) {
          $('#'+name+(i?'-'+i:'')).attr('disabled', !on);
        }
      }
      else {
        $elm.prev().attr('disabled', !on);
      }
    },

    /**
     * Get the current value of a #define (from the config text)
     */
    defineValue: function(name) {
      this.log('defineValue:'+name,4);
      var inf = $('#'+name)[0].defineInfo;
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
      var inf = $('#'+name)[0].defineInfo;
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
      var inf = $('#'+name)[0].defineInfo;
      if (inf) {
        var slash = val ? '' : '//';
        var newline = inf.line
          .replace(/^([ \t]*)(\/\/)([ \t]*)/, '$1$3')              // remove slashes
          .replace(inf.pre+inf.define, inf.pre+slash+inf.define);  // add them back
        this.setDefineLine(name, newline);
      }
    },

    /**
     * Update a #define (from the form) by altering the config text
     */
    updateDefineFromField: function(name) {
      this.log('updateDefineFromField:'+name,4);

      // Drop the suffix on sub-fields
      name = name.replace(/-\d+$/, '');

      var $elm = $('#'+name), inf = $elm[0].defineInfo;
      if (inf == null) return;

      var isCheck = $elm.attr('type') == 'checkbox',
          val = isCheck ? $elm.prop('checked') : $elm.val().trim();

      var newline;
      switch(inf.type) {
        case 'switch':
          var slash = val ? '' : '//';
          newline = inf.line.replace(inf.repl, '$1'+slash+'$3');
          break;
        case 'list':
        case 'quoted':
        case 'plain':
          if (isCheck) this.setMessage(name + ' should not be a checkbox!', 'error');
        case 'toggle':
          if (isCheck) {
            val = val ? inf.options[1] : inf.options[0];
          }
          else {
            if (inf.type == 'list')
              for (var i=1; i<inf.size; i++) val += ', ' + $('#'+name+'-'+i).val().trim();
          }
          newline = inf.line.replace(inf.repl, '$1'+(''+val).replace('$','\\$')+'$3');
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
      var inf = $('#'+name)[0].defineInfo;
      var $c = $(inf.field), txt = $c.text();

      var hilite_token = '[HIGHLIGHTER-TOKEN]';

      txt = txt.replace(inf.line, hilite_token + newline);
      inf.line = newline;

      // Convert txt into HTML before storing
      var html = txt.toHTML().replace(hilite_token, '<span></span>');

      // Set the final text including the highlighter
      $c.html(html);

      // Scroll to reveal the define
      if ($c.is(':visible')) this.scrollToDefine(name);
    },

    /**
     * Scroll a pre box to reveal a #define
     */
    scrollToDefine: function(name, always) {
      this.log('scrollToDefine:'+name,4);
      var inf = $('#'+name)[0].defineInfo, $c = $(inf.field);

      // Scroll to the altered text if it isn't visible
      var halfHeight = $c.height()/2, scrollHeight = $c.prop('scrollHeight'),
          lineHeight = scrollHeight/(inf.adv ? total_config_adv_lines : total_config_lines),
          textScrollY = (inf.lineNum * lineHeight - halfHeight).limit(0, scrollHeight - 1);

      if (always || Math.abs($c.prop('scrollTop') - textScrollY) > halfHeight) {
        $c.find('span').height(lineHeight);
        $c.animate({ scrollTop: textScrollY });
      }
    },

    /**
     * Set a form field to the current #define value in the config text
     */
    setFieldFromDefine: function(name) {
      var $elm = $('#'+name), inf = $elm[0].defineInfo,
          val = this.defineValue(name);

      this.log('setFieldFromDefine:' + name + ' to ' + val, 2);

      // If the item has a checkbox then set enabled state too
      var $cb = $('#'+name+'-switch'), on = true;
      if ($cb.length) {
        on = self.defineIsEnabled(name);
        $cb.prop('checked', on);
      }

      if (inf.type == 'list') {
        $.each(val.split(','),function(i,v){
          var $e = i > 0 ? $('#'+name+'-'+i) : $elm;
          $e.val(v.trim());
          $e.attr('disabled', !on);
        });
      }
      else {
        if (inf.type == 'toggle') val = val == inf.options[1];
        $elm.attr('type') == 'checkbox' ? $elm.prop('checked', val) : $elm.val(''+val);
        $elm.attr('disabled', !on); // enable/disable the form field (could also dim it)
      }
    },

    /**
     * Purge added fields and all their define info
     */
    purgeAddedFields: function(index) {
      $.each(define_list[index], function(){
        $('#'+this + ",[id^='"+this+"-'],label[for='"+this+"']").filter('.added').remove();
      });
      define_list[index] = [];
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
     *   - Pre-examine the #define for its prefix, value position, suffix, etc.
     *   - Construct RegExp's for the #define to quickly find (and replace) values.
     *   - Store the existing #define line as a fast key to finding it later.
     *   - Determine the line number of the #define so it can be scrolled to.
     *   - Gather nearby comments to be used as tooltips.
     *   - Look for JSON in nearby comments to use as select options.
     */
    getDefineInfo: function(name, adv) {
      if (adv === undefined) adv = false;
      this.log('getDefineInfo:'+name,4);
      var $c = adv ? $config_adv : $config,
          txt = $c.text();

      // a switch line with no value
      var findDef = new RegExp('^([ \\t]*//)?([ \\t]*#define[ \\t]+' + name + ')([ \\t]*/[*/].*)?$', 'm'),
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
        // a define with curly braces
        findDef = new RegExp('^(.*//)?(.*#define[ \\t]+' + name + '[ \\t]+)(\{[^\}]*\})([ \\t]*/[*/].*)?$', 'm');
        result = findDef.exec(txt);
        if (result !== null) {
          $.extend(info, {
            type:   'list',
            line:   result[0],
            pre:    result[1] === undefined ? '' : result[1].replace('//',''),
            define: result[2],
            size:   result[3].split(',').length,
            post:   result[4] === undefined ? '' : result[4]
          });
          info.regex = new RegExp('([ \\t]*//)?[ \\t]*' + info.define.regEsc() + '\{([^\}]*)\}' + info.post.regEsc(), 'm');
          info.repl  = new RegExp('(([ \\t]*//)?[ \\t]*' + info.define.regEsc() + '\{)[^\}]*(\}' + info.post.regEsc() + ')', 'm');
        }
        else {
          // a define with quotes
          findDef = new RegExp('^(.*//)?(.*#define[ \\t]+' + name + '[ \\t]+)("[^"]*")([ \\t]*/[*/].*)?$', 'm');
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
            findDef = new RegExp('^([ \\t]*//)?([ \\t]*#define[ \\t]+' + name + '[ \\t]+)(\\S*)([ \\t]*/[*/].*)?$', 'm');
            result = findDef.exec(txt);
            if (result !== null) {
              $.extend(info, {
                type:   'plain',
                line:   result[0],
                pre:    result[1] === undefined ? '' : result[1].replace('//',''),
                define: result[2],
                post:   result[4] === undefined ? '' : result[4]
              });
              if (result[3].match(/false|true/)) {
                info.type = 'toggle';
                info.options = ['false','true'];
              }
              info.regex = new RegExp('([ \\t]*//)?[ \\t]*' + info.define.regEsc() + '(\\S*)' + info.post.regEsc(), 'm');
              info.repl  = new RegExp('(([ \\t]*//)?[ \\t]*' + info.define.regEsc() + ')\\S*(' + info.post.regEsc() + ')', 'm');
            }
          }
        }
      }

      // Success?
      if (info.type) {
        // Get the end-of-line comment, if there is one
        var tooltip = '';
        findDef = new RegExp('.*#define[ \\t].*/[/*]+[ \\t]*(.*)');
        if (info.line.search(findDef) >= 0)
          tooltip = info.line.replace(findDef, '$1');

        // Get all the comments immediately before the item
        var r, s;
        findDef = new RegExp('(([ \\t]*(//|#)[^\n]+\n){1,4})([ \\t]*\n)?' + info.line.regEsc(), 'g');
        if (r = findDef.exec(txt)) {
          // Get the text of the found comments
          findDef = new RegExp('^[ \\t]*//+[ \\t]*(.*)[ \\t]*$', 'gm');
          while((s = findDef.exec(r[1])) !== null) {
            var tip = s[1].replace(/[ \\t]*(={5,}|@section[ \\t]+\w+)[ \\t]*/g, '');
            if (tip.length) {
              if (tip.match(/^#define[ \\t]/) != null) {
                tooltip = '';
                break;
              }
              // JSON data? Save as select options
              if (!info.options && tip.match(/:[\[{]/) != null) {
                // TODO
                // :[1-6] = value limits
                var o; eval('o=' + tip.substr(1));
                info.options = o;
                if (Object.prototype.toString.call(o) == "[object Array]" && o.length == 2 && !eval(''+o[0]))
                  info.type = 'toggle';
              }
              else {
                // Other lines added to the tooltip
                tooltip += ' ' + tip + '\n';
              }
            }
          }
        }


        // Add .tooltip and .lineNum properties to the info
        findDef = new RegExp('^'+name); // Strip the name from the tooltip
        $.extend(info, {
          tooltip: '<strong>'+name+'</strong> '+tooltip.trim().replace(findDef,'').toHTML(),
          lineNum: this.getLineNumberOfText(info.line, txt),
          switchable: (info.type != 'switch' && info.line.match(/^[ \t]*\/\//)) || false // Disabled? Mark as "switchable"
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

    /**
     * Add a temporary message to the page
     */
    setMessage: function(msg,type) {
      if (msg) {
        if (type === undefined) type = 'message';
        var $err = $('<p class="'+type+'">'+msg+'<span>x</span></p>').appendTo($msgbox), err = $err[0];
        var baseColor = $err.css('color').replace(/rgba?\(([^),]+,[^),]+,[^),]+).*/, 'rgba($1,');
        err.pulse_offset = (pulse_offset += 200);
        err.startTime = Date.now() + pulse_offset;
        err.pulser = setInterval(function(){
            var pulse_time = Date.now() + err.pulse_offset;
            var opac = 0.5+Math.sin(pulse_time/200)*0.4;
            $err.css({color:baseColor+(opac)+')'});
            if (pulse_time - err.startTime > 2500 && opac > 0.899) {
              clearInterval(err.pulser);
            }
          }, 50);
        $err.click(function(e) { $(this).remove(); return false; }).css({cursor:'pointer'});
      }
      else {
        $msgbox.find('p.error, p.warning').each(function() {
          if (this.pulser !== undefined && this.pulser)
            clearInterval(this.pulser);
          $(this).remove();
        });
      }
    },

    setRequestError: function(stat, path) {
      self.setMessage('Error '+stat+' – ' + path.replace(/^(https:\/\/[^\/]+\/)?.+(\/[^\/]+)$/, '$1...$2'), 'error');
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
