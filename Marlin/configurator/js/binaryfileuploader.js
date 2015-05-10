function BinaryFileUploader(o) {
	this.options = null;


	this._defaultOptions = {
		element: null, // HTML file element
		onFileLoad: function(file) {
			console.log(file.toString());
		}
	};


	this._init = function(o) {
		if (!this.hasFileUploaderSupport()) return;

		this._verifyDependencies();

		this.options = this._mergeObjects(this._defaultOptions, o);
		this._verifyOptions();

		this.addFileChangeListener();
	}


	this.hasFileUploaderSupport = function() {
		return !!(window.File && window.FileReader && window.FileList && window.Blob);
	}

	this.addFileChangeListener = function() {
		this.options.element.addEventListener(
			'change',
			this._bind(this, this.onFileChange)
		);
	}

	this.onFileChange = function(e) {
		// TODO accept multiple files
		var file = e.target.files[0],
		    reader = new FileReader();

		reader.onload = this._bind(this, this.onFileLoad);
		reader.readAsBinaryString(file);
	}

	this.onFileLoad = function(e) {
		var content = e.target.result,
		    string  = new BinaryString(content);
		this.options.onFileLoad(string);
	}


	this._mergeObjects = function(starting, override) {
		var merged = starting;
		for (key in override) merged[key] = override[key];

		return merged;
	}

	this._verifyOptions = function() {
		if (!(this.options.element && this.options.element.type && this.options.element.type === 'file')) {
			throw 'Invalid element param in options. Must be a file upload DOM element';
		}

		if (typeof this.options.onFileLoad !== 'function') {
			throw 'Invalid onFileLoad param in options. Must be a function';
		}
	}

	this._verifyDependencies = function() {
		if (!window.BinaryString) throw 'BinaryString is missing. Check that you\'ve correctly included it';
	}

	// helper function for binding methods to objects
	this._bind = function(object, method) {
		return function() {return method.apply(object, arguments);};
	}

	this._init(o);
}
