/*!
 * jQuery "stepper" Plugin
 * version 0.0.1
 * @requires jQuery v1.3.2 or later
 * @requires jCanvas
 *
 * Authored 2011-06-11 Scott Lahteine (thinkyhead.com)
 *
 *  A very simple numerical stepper.
 *  TODO: place arrows based on div size, make 50/50 width
 *
 *  Usage example:
 *
 *  $('#mydiv').jstepper({
 *    min: 1,
 *    max: 4,
 *    val: 1,
 *    arrowWidth: 15,
 *    arrowHeight: '22px',
 *    color: '#FFF',
 *    acolor: '#F70',
 *    hcolor: '#FF0',
 *    id: 'select-me',
 *    stepperClass: 'inner',
 *    textStyle: {width:'1.5em',fontSize:'20px',textAlign:'center'},
 *    onChange: function(v) { },
 *  });
 *
 */
;(function($) {
  var un = 'undefined';

  $.jstepperArrows = [
    { name:'prev', poly:[[1.0,0],[0,0.5],[1.0,1.0]] },
    { name:'next', poly:[[0,0],[1.0,0.5],[0,1.0]] }
  ];

 	$.fn.jstepper = function(args) {

		return this.each(function() {

      var defaults = {
        min: 1,
        max: null,
        val: null,
        active: true,
        placeholder: null,
        arrowWidth: 0,
        arrowHeight: 0,
        color: '#FFF',
        hcolor: '#FF0',
        acolor: '#F80',
        id: '',
        stepperClass: '',
        textStyle: '',
        onChange: (function(v){ if (typeof console.log !== 'undefined') console.log("val="+v); })
      };

      args = $.extend(defaults, args || {});

		  var min = args.min * 1,
          max = (args.max !== null) ? args.max * 1 : min,
          span = max - min + 1,
          val = (args.val !== null) ? args.val * 1 : min,
          active = !args.disabled,
          placeholder = args.placeholder,
          arrowWidth = 1 * args.arrowWidth.toString().replace(/px/,''),
          arrowHeight = 1 * args.arrowHeight.toString().replace(/px/,''),
          color = args.color,
          hcolor = args.hcolor,
          acolor = args.acolor,
			    $prev = $('<a href="#prev" style="cursor:w-resize;"><canvas/></a>'),
			    $marq = $('<div class="number"/>').css({float:'left',textAlign:'center'}),
			    $next = $('<a href="#next" style="cursor:e-resize;"><canvas/></a>'),
			    arrow = [ $prev.find('canvas')[0], $next.find('canvas')[0] ],
			    $stepper = $('<span class="jstepper"/>').append($prev).append($marq).append($next).append('<div style="clear:both;"/>'),
			    onChange = args.onChange;

      if (args.id) $stepper[0].id = args.id;
      if (args.stepperClass) $stepper.addClass(args.stepperClass);
      if (args.textStyle) $marq.css(args.textStyle);

      // replace a span, but embed elsewhere
      if (this.tagName == 'SPAN') {
        var previd = this.id;
        $(this).replaceWith($stepper);
        if (previd) $stepper.attr('id',previd);
      }
      else {
        $(this).append($stepper);
      }

      // hook to call functions on this object
      $stepper[0].ui = {

        refresh: function() {
          this.updateNumber();
          this._drawArrow(0, 1);
          this._drawArrow(1, 1);
          return this;
        },

        _drawArrow: function(i,state) {
          var $elm = $(arrow[i]),
              desc = $.jstepperArrows[i],
              fillStyle = (state == 2) ? hcolor : (state == 3) ? acolor : color,
              draw = { fillStyle: fillStyle },
              w = $elm.width(), h = $elm.height();

          if (w <= 0) w = $elm.attr('width');
          if (h <= 0) h = $elm.attr('height');

          $.each(desc.poly,function(i,v){
            ++i;
            draw['x'+i] = v[0] * w;
            draw['y'+i] = v[1] * h;
          });
          $elm.restoreCanvas().clearCanvas().drawLine(draw);
        },

        updateNumber: function() {
          $marq.html((active || placeholder === null) ? val.toString() : placeholder);
          return this;
        },

        _doclick: function(i) {
          this.add(i ? 1 : -1);
          this._drawArrow(i, 3);
          var self = this;
          setTimeout(function(){ self._drawArrow(i, 2); }, 50);
        },

        add: function(x) {
          val = (((val - min) + x + span) % span) + min;
          this.updateNumber();
          this.didChange(val);
          return this;
        },

        min: function(v) {
          if (typeof v === un) return min;
          this.setRange(v,max);
          return this;
        },

        max: function(v) {
          if (typeof v === un) return max;
          this.setRange(min,v);
          return this;
        },

        val: function(v) {
          if (typeof v === un) return val;
          val = (((v - min) + span) % span) + min;
          this.updateNumber();
          return this;
        },

        setRange: function(lo, hi, ini) {
          if (lo > hi) hi = (lo += hi -= lo) - hi;
          min = lo; max = hi; span = hi - lo + 1;
          if (typeof ini !== un) val = ini;
          if (val < min) val = min;
          if (val > max) val = max;
          this.updateNumber();
          return this;
        },

        active: function(a) {
          if (typeof a === un) return active;
          (active = a) ? $marq.removeClass('inactive') : $marq.addClass('inactive');
          this.updateNumber();
          return this;
        },

        disable: function() { this.active(false); return this; },
        enable: function() { this.active(true); return this; },

        clearPlaceholder: function() {
          this.setPlaceholder(null);
          return this;
        },
        setPlaceholder: function(p) {
          placeholder = p;
          if (!active) this.updateNumber();
          return this;
        },

        didChange: onChange

      };

      // set hover and click for each arrow
      $.each($.jstepperArrows, function(i,desc) {
        var $elm = $(arrow[i]),
            w = arrowWidth ? arrowWidth : $elm.width() ? $elm.width() : 15,
            h = arrowHeight ? arrowHeight : $elm.height() ? $elm.height() : 24;
        $elm[0]._index = i;
        $elm
          .css({float:'left'})
          .attr({width:w,height:h,'class':desc.name})
          .hover(
            function(e) { $stepper[0].ui._drawArrow(e.target._index, 2); },
            function(e) { $stepper[0].ui._drawArrow(e.target._index, 1); }
          )
          .click(function(e){
            $stepper[0].ui._doclick(e.target._index);
            return false;
          });
      });

      // init the visuals first time
  		$stepper[0].ui.refresh();

		}); // this.each

  }; // $.fn.jstepper

})( jQuery );

