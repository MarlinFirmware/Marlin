/*!
  jCanvas v2.2.1
  Caleb Evans
  2.2.1 revisions by Thinkyhead
*/

(function($, document, Math, Number, undefined) {

// jC global object
var jC = {};
jC.originals = {
	width: 20,
	height: 20,
	cornerRadius: 0,
	fillStyle: 'transparent',
	strokeStyle: 'transparent',
	strokeWidth: 5,
	strokeCap: 'butt',
	strokeJoin: 'miter',
	shadowX: 0,
	shadowY: 0,
	shadowBlur: 10,
	shadowColor: 'transparent',
	x: 0, y: 0,
	x1: 0, y1: 0,
	radius: 10,
	start: 0,
	end: 360,
	ccw: false,
	inDegrees: true,
	fromCenter: true,
	closed: false,
	sides: 3,
	angle: 0,
	text: '',
	font: 'normal 12pt sans-serif',
	align: 'center',
	baseline: 'middle',
	source: '',
	repeat: 'repeat'
};
// Duplicate original defaults
jC.defaults = $.extend({}, jC.originals);

// Set global properties
function setGlobals(context, map) {
	context.fillStyle = map.fillStyle;
	context.strokeStyle = map.strokeStyle;
	context.lineWidth = map.strokeWidth;
	context.lineCap = map.strokeCap;
	context.lineJoin = map.strokeJoin;
	context.shadowOffsetX = map.shadowX;
	context.shadowOffsetY = map.shadowY;
	context.shadowBlur = map.shadowBlur;
	context.shadowColor = map.shadowColor;
}

// Close path if chosen
function closePath(context, map) {
	if (map.closed === true) {
		context.closePath();
		context.fill();
		context.stroke();
	} else {
		context.fill();
		context.stroke();
		context.closePath();
	}
}

// Measure angles in degrees if chosen
function checkUnits(map) {
	if (map.inDegrees === true) {
		return Math.PI / 180;
	} else {
		return 1;
	}
}

// Set canvas defaults
$.fn.canvas = function(args) {
	// Reset defaults if no value is passed
	if (typeof args === 'undefined') {
		jC.defaults = jC.originals;
	} else {
		jC.defaults = $.extend({}, jC.defaults, args);
	}
	return this;
};

// Load canvas
$.fn.loadCanvas = function(context) {
	if (typeof context === 'undefined') {context = '2d';}
	return this[0].getContext(context);
};

// Create gradient
$.fn.gradient = function(args) {
	var ctx = this.loadCanvas(),
		// Specify custom defaults
		gDefaults = {
			x1: 0, y1: 0,
			x2: 0, y2: 0,
			r1: 10, r2: 100
		},
		params = $.extend({}, gDefaults, args),
		gradient, stops = 0, percent, i;
		
	// Create radial gradient if chosen
	if (typeof args.r1 === 'undefined' && typeof args.r2 === 'undefined') {
		gradient = ctx.createLinearGradient(params.x1, params.y1, params.x2, params.y2);
	} else {
		gradient = ctx.createRadialGradient(params.x1, params.y1, params.r1, params.x2, params.y2, params.r2);
	}
	
	// Count number of color stops
	for (i=1; i<=Number.MAX_VALUE; i+=1) {
		if (params['c' + i]) {
			stops += 1;
		} else {
			break;
		}
	}
	
	// Calculate color stop percentages if absent
	for (i=1; i<=stops; i+=1) {
		percent = Math.round((100 / (stops-1)) * (i-1)) / 100;
		if (typeof params['s' + i] === 'undefined') {
			params['s' + i] = percent;
		}
		gradient.addColorStop(params['s' + i], params['c' + i]);
	}
	return gradient;
};

// Create pattern
$.fn.pattern = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		pattern,
		img = document.createElement('img');
	img.src = params.source;
	
	// Create pattern
	function create() {
		if (img.complete === true) {
			// Create pattern
			pattern = ctx.createPattern(img, params.repeat);
		} else {
			throw "The pattern has not loaded yet";
		}
	}
	try {
		create();
	} catch(error) {
		img.onload = create;
	}
	return pattern;
};

// Clear canvas
$.fn.clearCanvas = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args);

	// Draw from center if chosen
	if (params.fromCenter === true) {
		params.x -= params.width / 2;
		params.y -= params.height / 2;
	}

	// Clear entire canvas if chosen
	ctx.beginPath();
	if (typeof args === 'undefined') {
		ctx.clearRect(0, 0, this.width(), this.height());
	} else {
		ctx.clearRect(params.x, params.y, params.width, params.height);
	} 
	ctx.closePath();
	return this;
};

// Save canvas
$.fn.saveCanvas = function() {
	var ctx = this.loadCanvas();
	ctx.save();
	return this;
};

// Restore canvas
$.fn.restoreCanvas = function() {
	var ctx = this.loadCanvas();
	ctx.restore();
	return this;
};

// Scale canvas
$.fn.scaleCanvas = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args);
	ctx.save();
	ctx.translate(params.x, params.y);
	ctx.scale(params.width, params.height);
	ctx.translate(-params.x, -params.y)
	return this;
};

// Translate canvas
$.fn.translateCanvas = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args);
	ctx.save();
	ctx.translate(params.x, params.y);		
	return this;
};

// Rotate canvas
$.fn.rotateCanvas = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		toRad = checkUnits(params);
	
	ctx.save();
	ctx.translate(params.x, params.y);
	ctx.rotate(params.angle * toRad);
	ctx.translate(-params.x, -params.y);
	return this;
};

// Draw rectangle
$.fn.drawRect = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		toRad = checkUnits(params),
		x1, y1, x2, y2, r;
	setGlobals(ctx, params);
	
	// Draw from center if chosen
	if (params.fromCenter === true) {
		params.x -= params.width / 2;
		params.y -= params.height / 2;
	}
		
	// Draw rounded rectangle if chosen
	if (params.cornerRadius > 0) {
		x1 = params.x;
		y1 = params.y;
		x2 = params.x + params.width;
		y2 = params.y + params.height;
		r = params.cornerRadius;
		if ((x2 - x1) - (2 * r) < 0) {
			r = (x2 - x1) / 2;
		}
		if ((y2 - y1) - (2 * r) < 0) {
			r = (y2 - y1) / 2;
		}
		ctx.beginPath();
		ctx.moveTo(x1+r,y1);
		ctx.lineTo(x2-r,y1);
		ctx.arc(x2-r, y1+r, r, 270*toRad, 360*toRad, false);
		ctx.lineTo(x2,y2-r);
		ctx.arc(x2-r, y2-r, r, 0, 90*toRad, false);
		ctx.lineTo(x1+r,y2);
		ctx.arc(x1+r, y2-r, r, 90*toRad, 180*toRad, false);
		ctx.lineTo(x1,y1+r);
		ctx.arc(x1+r, y1+r, r, 180*toRad, 270*toRad, false);
		ctx.fill();
		ctx.stroke();
		ctx.closePath();
	} else {
		ctx.fillRect(params.x, params.y, params.width, params.height);
		ctx.strokeRect(params.x, params.y, params.width, params.height);
	}
	return this;
};

// Draw arc
$.fn.drawArc = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		toRad = checkUnits(params);
		setGlobals(ctx, params);
	
	// Draw from center if chosen
	if (params.fromCenter === false) {
		params.x += params.radius;
		params.y += params.radius;
	}
	
	ctx.beginPath();
	ctx.arc(params.x, params.y, params.radius, (params.start*toRad)-(Math.PI/2), (params.end*toRad)-(Math.PI/2), params.ccw);
	// Close path if chosen
	closePath(ctx, params);
	return this;
};

// Draw ellipse
$.fn.drawEllipse = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		controlW = params.width * (4/3);
		setGlobals(ctx, params);
	
	// Draw from center if chosen
	if (params.fromCenter === false) {
		params.x += params.width / 2;
		params.y += params.height / 2;
	}
	
	// Increment coordinates to prevent negative values
	params.x += 1e-10;
	params.y += 1e-10;
	
	// Create ellipse
	ctx.beginPath();
	ctx.moveTo(params.x, params.y-params.height/2);
	ctx.bezierCurveTo(params.x-controlW/2,params.y-params.height/2,
		params.x-controlW/2,params.y+params.height/2,
		params.x,params.y+params.height/2);
	ctx.bezierCurveTo(params.x+controlW/2,params.y+params.height/2,
		params.x+controlW/2,params.y-params.height/2,
		params.x,params.y-params.height/2);
	ctx.closePath();
	ctx.fill();
	ctx.stroke();
	return this;
};

// Draw line
$.fn.drawLine = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		max = Number.MAX_VALUE, l,
		lx, ly;
	setGlobals(ctx, params);
	
	// Draw each point
	ctx.beginPath();
	ctx.moveTo(params.x1, params.y1);
	for (l=2; l<max; l+=1) {
		lx = params['x' + l];
		ly = params['y' + l];
		// Stop loop when all points are drawn
		if (typeof lx === 'undefined' || typeof ly === 'undefined') {
			break;
		}
		ctx.lineTo(lx, ly);
	}
	// Close path if chosen
	closePath(ctx, params);
	return this;
};

// Draw quadratic curve
$.fn.drawQuad = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		max = Number.MAX_VALUE, l,
		lx, ly, lcx, lcy;
	setGlobals(ctx, params);
	
	// Draw each point
	ctx.beginPath();
	ctx.moveTo(params.x1, params.y1);
	for (l=2; l<max; l+=1) {
		lx = params['x' + l];
    if (typeof lx === 'undefined') break;
		ly = params['y' + l];
    if (typeof ly === 'undefined') break;
		lcx = params['cx' + (l-1)];
    if (typeof lcx === 'undefined') break;
		lcy = params['cy' + (l-1)];
    if (typeof lcy === 'undefined') break;
		ctx.quadraticCurveTo(lcx, lcy, lx, ly);
	}
	// Close path if chosen
	closePath(ctx, params);
	return this;
};

// Draw Bezier curve
$.fn.drawBezier = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		max = Number.MAX_VALUE,
		l=2, lc=1, lx, ly, lcx1, lcy1, lcx2, lcy2, i;
	setGlobals(ctx, params);

	// Draw each point
	ctx.beginPath();
	ctx.moveTo(params.x1, params.y1);
	for (i=2; i<max; i+=1) {
		lx = params['x' + l];
    if (typeof lx === 'undefined') break;
		ly = params['y' + l];
    if (typeof ly === 'undefined') break;
		lcx1 = params['cx' + lc];
    if (typeof lcx1 === 'undefined') break;
		lcy1 = params['cy' + lc];
    if (typeof lcy1 === 'undefined') break;
		lcx2 = params['cx' + (lc+1)];
    if (typeof lcx2 === 'undefined') break;
		lcy2 = params['cy' + (lc+1)];
    if (typeof lcy2 === 'undefined') break;
		ctx.bezierCurveTo(lcx1, lcy1, lcx2, lcy2, lx, ly);
		l += 1;
		lc += 2;
	}
	// Close path if chosen
	closePath(ctx, params);
	return this;
};

// Draw text
$.fn.drawText = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args);
	setGlobals(ctx, params);
	
	// Set text-specific properties
	ctx.textBaseline = params.baseline;
	ctx.textAlign = params.align;
	ctx.font = params.font;
	
	ctx.strokeText(params.text, params.x, params.y);
	ctx.fillText(params.text, params.x, params.y);
	return this;
};

// Draw image
$.fn.drawImage = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		// Define image source
		img = document.createElement('img');
  	img.src = params.source;
	  setGlobals(ctx, params);

	// Draw image function
	function draw() {
		if (img.complete) {

  		var scaleFac = img.width / img.height;

			// If width/height are specified
			if (typeof args.width !== 'undefined' && typeof args.height !== 'undefined') {
				img.width = args.width;
				img.height = args.height;
			// If width is specified
			} else if (typeof args.width !== 'undefined' && typeof args.height === 'undefined') {
				img.width = args.width;
				img.height = img.width / scaleFac;
			// If height is specified
			} else if (typeof args.width === 'undefined' && typeof args.height !== 'undefined') {
				img.height = args.height;
				img.width = img.height * scaleFac;
			}
		
			// Draw from center if chosen
			if (params.fromCenter === true) {
				params.x -= img.width / 2;
				params.y -= img.height / 2;
			}

			// Draw image
			ctx.drawImage(img, params.x, params.y, img.width, img.height);
		} else {
			throw "The image has not loaded yet.";
		}
	}

  function dodraw() {
    // console.log("dodraw...");
    try {
  	  // console.log("dodraw...try...");
      draw();
    }
    catch(error) {
      // console.log("dodraw...catch: " + error);
    }
  }

	// Draw image if already loaded
  // console.log("--------------------");
  // console.log("drawImage " + img.src);
	try {
    // console.log("try...");
		draw();
	} catch(error) {
    // console.log("catch: " + error);
		img.onload = dodraw;
	}
	return this;
};

// Draw polygon
$.fn.drawPolygon = function(args) {
	var ctx = this.loadCanvas(),
		params = $.extend({}, jC.defaults, args),
		theta, dtheta, x, y,
		toRad = checkUnits(params), i;
	setGlobals(ctx, params);
	
	if (params.sides >= 3) {		
		// Calculate points and draw
		theta = (Math.PI/2) + (Math.PI/params.sides) + (params.angle*toRad);
		dtheta = (Math.PI*2) / params.sides;
		for (i=0; i<params.sides; i+=1) {
			x = params.x + (params.radius * Math.cos(theta)) + 1e-10;
			y = params.y + (params.radius * Math.sin(theta)) + 1e-10;
			if (params.fromCenter === false) {
				x += params.radius;
				y += params.radius;
			}
			ctx.lineTo(x, y);
			theta += dtheta;
		}
		closePath(ctx, params);
	}
	return this;
};

return window.jCanvas = jC;
}(jQuery, document, Math, Number));