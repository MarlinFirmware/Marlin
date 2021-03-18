(function(Chart) {
	var helpers = Chart.helpers;
	var plugins = Chart.plugins;
	Chart.defaults.global.animation.duration = 1000;

	Chart.defaults._set('linearGauge', {
		scale: {
			type: 'linearGauge',
			horizontal: false,
			range: {
				startValue: -100,
				endValue: 500
			},
			responsive: true,
			font: {
				fontName: 'Arial',
				fontSize: 12
			},
			axisWidth: 6,
			ticks: {
				majorTicks: {
					interval: 100,
					height: 1,
				}
			},
			scaleLabel: {
				display: true,
				interval: 100,
				units: '',
				customValues: [],
				offset: -10,
				color: '#777b80'
			}
		},
		padding: {
			top: 0,
			bottom: 0,
			left: 0,
			right: 0
		},
		tooltips: {
			callbacks: {
				label: function(tooltipItem, data) {
					var label = data.datasets[tooltipItem.datasetIndex].label || '';

					if (label) {
						label += ': ';
					}
					label += Math.round(data.datasets[tooltipItem.datasetIndex].data[0] * 100) / 100;
					return label;
				}
			}
		},
		legend: {
			display: true,
			labels: {
				fontColor: 'rgb(0, 0, 0)'
			},
			position: 'bottom'
		}
	});

	Chart.controllers.linearGauge = Chart.DatasetController.extend({

		dataElementType: Chart.elements.Gaugerect,

		initialize: function() {
			var me = this;
			var meta;

			Chart.DatasetController.prototype.initialize.apply(me, arguments);

			meta = me.getMeta();

		},

		linkScales: helpers.noop,

		update: function(reset) {
			var me = this;
			var rects = me.getMeta().data;
			var i, ilen;
			me.datashifts = 0;

			for (i = 0, ilen = rects.length; i < ilen; ++i) {
				me.updateElement(rects[i], i, me.datashifts);
				me.datashifts += 10;
			}
		},

		updateElement: function(rectangle, index, reset) {
			var me = this;
			var chart = me.chart;
			var meta = me.getMeta();
			var dataset = me.getDataset();

			var custom = rectangle.custom || {};
			var rectangleOptions = chart.options.elements.rectangle;
			var gaugeOptions = chart.options.elements.gaugerect;
			rectangle._Scale = me.getScaleForId(chart.options.scale.id || 'gaugescale');
			rectangle._datasetIndex = me.index;
			rectangle._index = index;
			rectangle.rangeColorImage = null;

			//	Init element model
			rectangle._model = {
				datasetLabel: dataset.label,
				label: chart.data.labels[index],
				borderSkipped: custom.borderSkipped ? custom.borderSkipped : rectangleOptions.borderSkipped,
				backgroundColor: custom.backgroundColor ? custom.backgroundColor : helpers.valueAtIndexOrDefault(dataset.backgroundColor, index, gaugeOptions.backgroundColor),
				borderColor: custom.borderColor ? custom.borderColor : helpers.valueAtIndexOrDefault(dataset.borderColor, index, rectangleOptions.borderColor),
				borderWidth: custom.borderWidth ? custom.borderWidth : helpers.valueAtIndexOrDefault(dataset.borderWidth, index, rectangleOptions.borderWidth)
			};

			//	Set empty view as start point for animation
			if(typeof rectangle._view === 'undefined') rectangle._view = {};

			me.updateElementGeometry(rectangle, index, reset);

		},

		updateElementGeometry: function(rectangle, index, reset) {
			var me = this;
			var model = rectangle._model;
			var start = rectangle._view;
			var dataset = me.getDataset().data;
			var dopt = me.getDataset();
			var chart = me.chart;
			var datasets = chart.data.datasets;
			var gaugeOptions = chart.options.elements.gaugerect;
			var vscale = me.getScaleForId(chart.options.scale.id || 'gaugescale');
			//var base = vscale.getBasePixel();
			var base = vscale.getBase();
			var horizontal = rectangle._Scale.isHorizontal();
			//var ruler = me._ruler || me.getRuler();
			var vpixels = me.calculateBarValuePixels(me.index, index, horizontal);

			model.horizontal = horizontal;
			model.base = base;
			model.head = vpixels.head;
			model.x = horizontal ? vpixels.base : vpixels.offset;
			model.y = horizontal ? (vpixels.offset - (dopt.width || gaugeOptions.width)) : vpixels.head;
			model.height = horizontal ? (dopt.width || gaugeOptions.width) : (vpixels.base - vpixels.head);
			model.width = horizontal ? (vpixels.head - vpixels.base) : (dopt.width || gaugeOptions.width);
			model.value = vscale.getRightValue(datasets[me.index].data[index]);

			model.scaleValue = 0;
			if (horizontal) {
				model.scaleValue = vscale.width / (vscale.options.range.endValue - vscale.options.range.startValue);
			} else {
				model.scaleValue = vscale.height / (vscale.options.range.endValue - vscale.options.range.startValue);
			}

			if(typeof start.x === 'undefined' && typeof start.y === 'undefined'){
				if(horizontal){
					start.x = vpixels.base;
					start.width = 0;
				} else {
					start.y = vpixels.base;
					start.height = 0;
				}
			}

		},

		calculateBarValuePixels: function(datasetIndex, index, horizontal) {
			var me = this;
			var chart = me.chart;
			var scale = me.getScaleForId(chart.options.scale.id || 'gaugescale');
			var datasets = chart.data.datasets;
			var dopt = datasets[datasetIndex];
			var value = scale.getRightValue(datasets[datasetIndex].data[index]);
			var stacked = scale.options.stacked;
			var start = 0;
			var i, imeta, ivalue, base, head, size, offset;

			base = scale.scalePoint(start);
			head = scale.scalePoint(start + value);
			size = (head - base) / 2;
			offset = horizontal ? scale.yCenter - dopt.offset : scale.xCenter + dopt.offset;

			return {
				size: size,
				base: base,
				head: head,
				center: head + size / 2,
				offset: offset
			};
		},

		draw: function() {
			var me = this;
			var chart = me.chart;
			var rects = me.getMeta().data;
			var dataset = me.getDataset();
			var ilen = rects.length;
			var i = 0;

			helpers.canvas.clipArea(chart.ctx, chart.chartArea);

			for (; i < ilen; ++i) {
				if (!isNaN(dataset.data[i])) {
					rects[i].draw();
				}
			}

			helpers.canvas.unclipArea(chart.ctx);
		},

		setHoverStyle: function(rectangle) {
			var dataset = this.chart.data.datasets[rectangle._datasetIndex];
			var index = rectangle._index;
			var custom = rectangle.custom || {};
			var model = rectangle._model;

			model.backgroundColor = custom.hoverBackgroundColor ? custom.hoverBackgroundColor : helpers.valueAtIndexOrDefault(dataset.hoverBackgroundColor, index, helpers.getHoverColor(model.backgroundColor));
			model.borderColor = custom.hoverBorderColor ? custom.hoverBorderColor : helpers.valueAtIndexOrDefault(dataset.hoverBorderColor, index, helpers.getHoverColor(model.borderColor));
			model.borderWidth = custom.hoverBorderWidth ? custom.hoverBorderWidth : helpers.valueAtIndexOrDefault(dataset.hoverBorderWidth, index, model.borderWidth);
		},

		removeHoverStyle: function(rectangle) {
			var dataset = this.chart.data.datasets[rectangle._datasetIndex];
			var index = rectangle._index;
			var custom = rectangle.custom || {};
			var model = rectangle._model;
			var rectangleElementOptions = this.chart.options.elements.gaugerect;

			model.backgroundColor = custom.backgroundColor ? custom.backgroundColor : helpers.valueAtIndexOrDefault(dataset.backgroundColor, index, rectangleElementOptions.backgroundColor);
			model.borderColor = custom.borderColor ? custom.borderColor : helpers.valueAtIndexOrDefault(dataset.borderColor, index, rectangleElementOptions.borderColor);
			model.borderWidth = custom.borderWidth ? custom.borderWidth : helpers.valueAtIndexOrDefault(dataset.borderWidth, index, rectangleElementOptions.borderWidth);
		}

	});
}).call(this, Chart);
