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

    init: function() {
      self = this; // a 'this' for use when 'this' is something else

      // Make a droppable file uploader
      var $uploader = $('#file-upload');
      var fileUploader = new BinaryFileUploader({
        element:    $uploader[0],
        onFileLoad: function(file) { console.log(this); self.handleFileLoad(file, $uploader); }
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
      var r, findDef = new RegExp('[ \\t]*#define[ \\t]+(BOARD_[^ \\t]+)[ \\t]+(\\d+)[ \\t]*(//[ \\t]*)?(.+)?', 'gm');
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
          console.log("Can't parse "+filename);
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
        stepperClass: 'inner',
        textStyle: {width:'1.5em',fontSize:'120%',textAlign:'center'},
        // onChange: function(v) { },
      });
*/

      // prettyPrint();
    },

    initField: function(name) {
      var $elm = $('#'+name), isText = $elm.attr('type') == 'text';
      this.setFieldFromDefine(name);
      isText ? $elm.bind('input', this.handleChange) : $elm.change(this.handleChange)
    },

    handleChange: function(e) {
      self.updateDefineForField(e.target);
    },

    handleSwitch: function(e) {
      var $elm = $(e.target), $prev = $elm.prev();
      var on = $elm.prop('checked') || false;
      $prev.attr('disabled', !on);
      self.setDefineEnabled($prev[0], on);
    },

    setDefineEnabled: function(elm, val) {
      var $elm = $(elm);

      // console.log("Enable: " + elm.id + " = " + val);

      var txt = $config.text();

      var findDef = new RegExp('^[ \\t]*(//[ \\t]*)?(#define[ \\t]+' + elm.id + '([ \\t].*)?)$', 'm');
      txt = txt.replace(findDef, val ? '$2': '//$2');

      // Now set the text in the field
      $config.text(txt);
    },

    updateDefineForField: function(elm) {
      var $elm = $(elm),
          isCheck = $elm.attr('type') == 'checkbox',
          val = isCheck ? $elm.prop('checked') : $elm.val();

      // console.log("Set: " + elm.id + " = " + val);

      var txt = $config.text();

      if (isCheck) {
        var findDef = new RegExp('^([ \\t]*)(//[ \\t]*)?(#define[ \\t]+' + elm.id + '([ \\t].*)?)$', 'm');
        txt = txt.replace(findDef, val ? '$1$3': '$1//$3');
      }
      else {
        // Match the define name, 1 = "#define name ", 3=value, 4=comment
        var findDef = new RegExp('^([ \\t]*(//)?[ \\t]*#define[ \\t]+' + elm.id + '[ \\t]+)(.*)([ \\t]*(//)?.*)$', 'm');
        if ($elm.hasClass('quote')) val = '"' + val + '"'
        txt = txt.replace(findDef, '$1!!REGEXP!!$4').replace('!!REGEXP!!', val);
      }
      // Now set the text in the field
      $config.text(txt);
    },

    setFieldFromDefine: function(name, adv) {
      var $elm = $('#'+name), elm = $elm[0];
      var isCheck = $elm.attr('type') == 'checkbox';
      var val = this.defineValue(name, adv);

      isCheck ? $elm.prop('checked', val) : $elm.val("" + val);

      // If the item has a checkbox then set enabled state too
      var $cb = $('#'+name+'-switch');
      if ($cb.length) {
        var on = self.defineIsEnabled(name,adv);
        $elm.attr('disabled', !on);
        $cb.prop('checked', on);
      }
    },

    defineValue: function(name, adv) {
      var $elm = $('#'+name), elm = $elm[0];
      var $c = adv ? $config_adv : $config;
      if ($elm.attr('type') == 'checkbox') {
        // maybe spaces, maybe comment, #define, spaces, name, maybe a comment
        var findDef = new RegExp('^[ \\t]*(//[ \\t]*)?(#define[ \\t]+' + elm.id + '([ \\t]*(//)?.*)?)$', 'm');
        var result = findDef.exec($c.text());
        return (result ? result[1] == undefined || result[1].trim() != '//' : false);
      }
      else {
        // maybe spaces, maybe comment, #define, spaces, name, one or more spaces, value, maybe a comment
        var findDef = new RegExp('^([ \\t]*(//[ \\t]*)?#define[ \\t]+' + elm.id + '[ \\t]+)(.*)([ \\t]*(//)?.*)$', 'm');
        var result = findDef.exec($c.text());
        if (result !== null) {
          var val = result[3];
          if (val.search(/^".*"$/) >= 0) $elm.addClass('quote');
          if ($elm.hasClass('quote')) val = val.replace(/^"(.*)"$/, '$1');
          return val;
        }
        return 'fail';
      }
    },

    defineIsEnabled: function(name, adv) {
      var $elm = $('#'+name);
      var $c = adv ? $config_adv : $config;
      var findDef = new RegExp('^[ \\t]*(//[ \\t]*)?(#define[ \\t]+' + name + '([ \\t]*(//)?.*)?)$', 'm');
      var result = findDef.exec($c.text());
      return (result ? result[1].trim() != '//' : false);
    },

    logOnce: function(o) {
      if (typeof o.didLogThisObject === 'undefined') {
        console.log(o);
        o.didLogThisObject = true;
      }
    },

    EOF: null
  };

})();

// Typically the app would be in its own file, but this would be here
configuratorApp.init();

});
