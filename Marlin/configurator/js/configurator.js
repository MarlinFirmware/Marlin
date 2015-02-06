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

var marlin_config = '..';

// Extend String
String.prototype.lpad = function(len, chr) {
  if (chr === undefined) { chr = '&nbsp;'; }
  var s = this+'', need = len - s.length;
  if (need > 0) { s = new Array(need+1).join(chr) + s; }
  return s;
};

String.prototype.prePad = function(len, chr) {
  return len ? this.lpad(len, chr) : this;
};

String.prototype.zeroPad = function(len) {
  return len ? this.prePad(len, '0') : this;
};

String.prototype.regEsc = function() {
  return this.replace(/[.?*+^$[\]\\(){}|-]/g, "\\$&");
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
  }
});

// The app is a singleton
var configuratorApp = (function(){

  // private variables and functions go here
  var self,
      pi2 = Math.PI * 2,
      boards_file = 'boards.h',
      config_file = 'Configuration.h',
      config_adv_file = 'Configuration_adv.h',
      $config = $('#config_text'),
      $config_adv = $('#config_adv_text'),
      boards_list = {},
      therms_list = {};

  // Return this anonymous object as configuratorApp
  return {
    my_public_var: 4,
    logging: 1,

    init: function() {
      self = this; // a 'this' for use when 'this' is something else

      // Make a droppable file uploader
      var $uploader = $('#file-upload');
      var fileUploader = new BinaryFileUploader({
        element:    $uploader[0],
        onFileLoad: function(file) { self.handleFileLoad(file, $uploader); }
      });

      if (!fileUploader.hasFileUploaderSupport()) alert('Your browser doesn\'t support the file reading API');

      // Read boards.h
      boards_list = {};

      var errFunc = function(jqXHR, textStatus, errorThrown) {
                      alert('Failed to load '+this.url+'. Try the file field.');
                    };

      $.ajax({
        url: marlin_config+'/'+boards_file,
        type: 'GET',
        async: false,
        cache: false,
        success: function(txt) {
          // Get all the boards and save them into an object
          self.initBoardsFromText(txt);
        },
        error: errFunc
      });

      // Read Configuration.h
      $.ajax({
        url: marlin_config+'/'+config_file,
        type: 'GET',
        async: false,
        cache: false,
        success: function(txt) {
          // File contents into the textarea
          $config.text(txt);
          // Get thermistor info too
          self.initThermistorsFromText(txt);
        },
        error: errFunc
      });

      // Read Configuration.h
      $.ajax({
        url: marlin_config+'/'+config_adv_file,
        type: 'GET',
        async: false,
        cache: false,
        success: function(txt) {
          // File contents into the textarea
          $config_adv.text(txt);
          self.setupConfigForm();
        },
        error: errFunc
      });

    },

    initBoardsFromText: function(txt) {
      boards_list = {};
      var r, findDef = new RegExp('[ \\t]*#define[ \\t]+(BOARD_[\\w_]+)[ \\t]+(\\d+)[ \\t]*(//[ \\t]*)?(.+)?', 'gm');
      while((r = findDef.exec(txt)) !== null) {
        boards_list[r[1]] = r[2].prePad(3, '  ') + " — " + r[4].replace(/\).*/, ')');
      }
    },

    initThermistorsFromText: function(txt) {
      // Get all the thermistors and save them into an object
      var r, s, findDef = new RegExp('(//.*\n)+\\s+(#define[ \\t]+TEMP_SENSOR_0)', 'g');
      r = findDef.exec(txt);
      findDef = new RegExp('^//[ \\t]*([-\\d]+)[ \\t]+is[ \\t]+(.*)[ \\t]*$', 'gm');
      while((s = findDef.exec(r[0])) !== null) {
        therms_list[s[1]] = s[1].prePad(4, '  ') + " — " + s[2];
      }
    },

    handleFileLoad: function(file, $uploader) {
      file += '';
      var filename = $uploader.val().replace(/.*[\/\\](.*)$/, '$1');
      switch(filename) {
        case config_file:
          $config.text(file);
          this.initThermistorsFromText(file);
          this.refreshConfigForm();
          break;
        case config_adv_file:
          $config_adv.text(file);
          this.refreshConfigForm();
          break;
        case boards_file:
          this.initBoardsFromText(file);
          $('#MOTHERBOARD').html('').addOptions(boards_list);
          this.initField('MOTHERBOARD');
          break;
        default:
          this.log("Can't parse "+filename, 1);
          break;
      }
    },

    setupConfigForm: function() {
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
        var $label = $(this).prev();
        if ($label[0].tagName == 'LABEL') {
          $label.attr('for',name);
        }
      });

      // Get all 'switchable' class items and add a checkbox
      $('#config_form .switchable').each(function(){
        $(this).after(
          $('<input>',{type:'checkbox',value:'1',class:'enabler'}).prop('checked',true)
            .attr('id',this.id + '-switch')
            .change(self.handleSwitch)
        );
      });

      $('#SERIAL_PORT').addOptions([0,1,2,3,4,5,6,7]);
      $('#BAUDRATE').addOptions([2400,9600,19200,38400,57600,115200,250000]);
      $('#MOTHERBOARD').addOptions(boards_list);
      $('#EXTRUDERS').addOptions([1,2,3,4]);
      $('#POWER_SUPPLY').addOptions({'1':'ATX','2':'Xbox 360'});

      this.refreshConfigForm();
    },

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

      $('#serial_stepper').jstepper({
        min: 0,
        max: 7,
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


      // prettyPrint();
    },

    /**
     * initField - make a field responsive and get info
     * about its configuration file define
     */
    initField: function(name) {
      var $elm = $('#'+name), elm = $elm[0];
      if (elm.configInfo === undefined) {
        elm.configInfo = this.getDefineInfo(name);
        $elm.on($elm.attr('type') == 'text' ? 'input' : 'change', this.handleChange);
      }
      this.setFieldFromDefine(name);
    },

    handleChange: function(e) {
      self.updateDefineForField(e.target.id);
    },

    handleSwitch: function(e) {
      var $elm = $(e.target), $prev = $elm.prev();
      var on = $elm.prop('checked') || false;
      $prev.attr('disabled', !on);
      self.setDefineEnabled($prev[0].id, on);
    },

    setDefineEnabled: function(name, val) {
      this.log('setDefineEnabled:'+name,3);

      var $elm = $('#'+name), elm = $elm[0], inf = elm.configInfo;
      var $c = $config; // for now
      var txt = $c.text();

      var slash = val ? '' : '//';
      var newline = inf.line
        .replace(/^([ \t]*)(\/\/)([ \t]*)/, '$1$3') // remove slashes
        .replace(inf.pre+inf.define, inf.pre+slash+inf.define);     // add them back

      txt = txt.replace(inf.line, newline);
      inf.line = newline;
      this.log(newline, 2);

      $c.text(txt);
    },

    defineIsEnabled: function(name, adv) {
      this.log('defineIsEnabled:'+name,4);
      var $elm = $('#'+name), elm = $elm[0];
      var $c = adv ? $config_adv : $config;

      var result = elm.configInfo.regex.exec($c.text());
      this.log(result,2);

      var on = result !== null ? result[1].trim() != '//' : true;
      this.log(name + ' = ' + on, 4);

      return on;
    },

    updateDefineForField: function(name, adv) {
      this.log('updateDefineForField:'+name,4);
      var $elm = $('#'+name), elm = $elm[0], inf = elm.configInfo;
      var $c = adv ? $config_adv : $config;
      var txt = $c.text();

      // var result = inf.repl.exec(txt);
      // this.log(result, 2);

      var isCheck = $elm.attr('type') == 'checkbox',
          val = isCheck ? $elm.prop('checked') : $elm.val();

      var newline;
      switch(inf.type) {
        case 'switch':
          var slash = val ? '' : '//';
          newline = (inf.pre + slash + inf.define + inf.post);
          break;
        case 'quoted':
          if (isCheck) {
            this.log(name + ' should not be a checkbox', 1);
            var slash = val ? '' : '//';
            newline = (inf.pre + slash + inf.define + '"'+val+'"' + inf.post);
          }
          else {
            newline = inf.pre + inf.define + '"'+val+'"' + inf.post;
          }
          break;
        case 'plain':
          if (isCheck) {
            this.log(name + ' should not be a checkbox', 1);
            var slash = val ? '' : '//';
            newline = (inf.pre + slash + inf.define + val + inf.post);
          }
          else {
            newline = inf.pre + inf.define + val + inf.post;
          }
          break;
      }

      txt = txt.replace(inf.line, newline);
      inf.line = newline;
      this.log(newline, 2);

      $c.text(txt);
    },

    setFieldFromDefine: function(name, adv) {
      var $elm = $('#'+name), elm = $elm[0];
      var isCheck = $elm.attr('type') == 'checkbox';
      var val = this.defineValue(name, adv);

      this.log('setFieldFromDefine:' + name + ' to ' + val, 4);

      isCheck ? $elm.prop('checked', val) : $elm.val("" + val);

      // If the item has a checkbox then set enabled state too
      var $cb = $('#'+name+'-switch');
      if ($cb.length) {
        var on = self.defineIsEnabled(name,adv);
        $elm.attr('disabled', !on);
        $cb.prop('checked', on);
      }
    },

    getDefineInfo: function(name, adv) {
      this.log('getDefineInfo:'+name,4);
      var $elm = $('#'+name), elm = $elm[0];
      var $c = adv ? $config_adv : $config;

      // a switch line with no value
      var findDef = new RegExp('^(.*//)?(.*#define[ \\t]+' + elm.id + ')([ \\t]*/[*/].*)?$', 'm');
      var result = findDef.exec($c.text());
      if (result !== null) {
        var info = {
          type: 'switch',
          line: result[0], // whole line
          pre: result[1] === undefined ? '' : result[1].replace('//',''),
          define: result[2],
          post: result[3] === undefined ? '' : result[3]
        };
        info.regex = new RegExp('(.*//)?(.*' + info.define.regEsc() + info.post.regEsc() + ')', 'm');
        info.repl = info.regex;
        this.log(info,2);
        return info;
      }

      // a define with quotes
      findDef = new RegExp('^(.*//)?(.*#define[ \\t]+' + elm.id + '[ \\t]+)("[^"]*")([ \\t]*/[*/].*)?$', 'm');
      result = findDef.exec($c.text());
      if (result !== null) {
        var info = {
          type: 'quoted',
          line: result[0],
          pre: result[1] === undefined ? '' : result[1].replace('//',''),
          define: result[2],
          post: result[4] === undefined ? '' : result[4]
        };
        info.regex = new RegExp('(.*//)?.*' + info.define.regEsc() + '"([^"]*)"' + info.post.regEsc(), 'm');
        info.repl  = new RegExp('((.*//)?.*' + info.define.regEsc() + '")[^"]*("' + info.post.regEsc() + ')', 'm');
        this.log(info,2);
        return info;
      }

      // a define with no quotes
      findDef = new RegExp('^(.*//)?(.*#define[ \\t]+' + elm.id + '[ \\t]+)(\\S*)([ \\t]*/[*/].*)?$', 'm');
      result = findDef.exec($c.text());
      if (result !== null) {
        var info = {
          type: 'plain',
          line: result[0],
          pre: result[1] === undefined ? '' : result[1].replace('//',''),
          define: result[2],
          post: result[4] === undefined ? '' : result[4]
        };
        info.regex = new RegExp('(.*//)?.*' + info.define.regEsc() + '(\\S*)' + info.post.regEsc(), 'm');
        info.repl = new RegExp('((.*//)?.*' + info.define.regEsc() + ')\\S*(' + info.post.regEsc() + ')', 'm');
        this.log(info,2);
        return info;
      }

      return null;
    },

    defineValue: function(name, adv) {
      this.log('defineValue:'+name,4);
      var $elm = $('#'+name), elm = $elm[0];
      var $c = adv ? $config_adv : $config;
      var inf = elm.configInfo;

      var result = inf.regex.exec($c.text());
      this.log(result,2);
      switch(inf.type) {
        case 'switch': return result[1] != '//';
        case 'quoted': return result[2];
        case 'plain':  return result[2];
      }
    },

    log: function(o,l) {
      if (l === undefined) l = 0;
      if (this.logging>=l*1) console.log(o);
    },

    logOnce: function(o) {
      if (typeof o.didLogThisObject === 'undefined') {
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
