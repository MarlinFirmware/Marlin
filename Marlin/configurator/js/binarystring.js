function BinaryString(source) {
	this._source = null;
	this._bytes  = [];
	this._pos    = 0;
	this._length = 0;

	this._init = function(source) {
		this._source = source;
		this._bytes  = this._stringToBytes(this._source);
		this._length = this._bytes.length;
	}

	this.current = function() {return this._pos;}

	this.rewind  = function() {return this.jump(0);}
	this.end     = function() {return this.jump(this.length()  - 1);}
	this.next    = function() {return this.jump(this.current() + 1);}
	this.prev    = function() {return this.jump(this.current() - 1);}

	this.jump = function(pos) {
		if (pos < 0 || pos >= this.length()) return false;

		this._pos = pos;
		return true;
	}

	this.readByte = function(pos) {
		pos = (typeof pos == 'number') ? pos : this.current();
		return this.readBytes(1, pos)[0];
	}

	this.readBytes = function(length, pos) {
		length = length || 1;
		pos    = (typeof pos == 'number') ? pos : this.current();

		if (pos          >  this.length() ||
		    pos          <  0             ||
		    length       <= 0             ||
		    pos + length >  this.length() ||
		    pos + length <  0
		) {
			return false;
		}

		var bytes = [];
		
		for (var i = pos; i < pos + length; i++) {
			bytes.push(this._bytes[i]);
		}

		return bytes;
	}

	this.length = function() {return this._length;}

	this.toString = function() {
		var string = '',
		    length = this.length();

		for (var i = 0; i < length; i++) {
			string += String.fromCharCode(this.readByte(i));
		}

		return string;
	}

	this.toUtf8 = function() {
		var inc    = 0,
		    string = '',
		    length = this.length();

		// determine if first 3 characters are the BOM
		// then skip them in output if so
		if (length >= 3               &&
		    this.readByte(0) === 0xEF &&
		    this.readByte(1) === 0xBB &&
		    this.readByte(2) === 0xBF
		) {
			inc = 3;
		}

		for (; inc < length; inc++) {
			var byte1 = this.readByte(inc),
			    byte2 = 0,
			    byte3 = 0,
			    byte4 = 0,
			    code1 = 0,
			    code2 = 0,
			    point = 0;

			switch (true) {
				// single byte character; same as ascii
				case (byte1 < 0x80):
					code1 = byte1;
					break;

				// 2 byte character
				case (byte1 >= 0xC2 && byte1 < 0xE0):
					byte2 = this.readByte(++inc);

					code1 = ((byte1 & 0x1F) << 6) +
					         (byte2 & 0x3F);
					break;

				// 3 byte character
				case (byte1 >= 0xE0 && byte1 < 0xF0):
					byte2 = this.readByte(++inc);
					byte3 = this.readByte(++inc);

					code1 = ((byte1 & 0xFF) << 12) +
					        ((byte2 & 0x3F) <<  6) +
					         (byte3 & 0x3F);
					break;

				// 4 byte character
				case (byte1 >= 0xF0 && byte1 < 0xF5):
					byte2 = this.readByte(++inc);
					byte3 = this.readByte(++inc);
					byte4 = this.readByte(++inc);

					point = ((byte1 & 0x07) << 18) +
					        ((byte2 & 0x3F) << 12) +
					        ((byte3 & 0x3F) <<  6) +
					         (byte4 & 0x3F)
					point -= 0x10000;

					code1 = (point >> 10)    + 0xD800;
					code2 = (point &  0x3FF) + 0xDC00;
					break;

				default:
					throw 'Invalid byte ' + this._byteToString(byte1) + ' whilst converting to UTF-8';
					break;
			}

			string += (code2) ? String.fromCharCode(code1, code2)
			                  : String.fromCharCode(code1);
		}

		return string;
	}

	this.toArray  = function() {return this.readBytes(this.length() - 1, 0);} 


	this._stringToBytes = function(str) {
		var bytes = [],
		    chr   = 0;

		for (var i = 0; i < str.length; i++) {
			chr = str.charCodeAt(i);
			bytes.push(chr & 0xFF);
		}

		return bytes;
	}

	this._byteToString = function(byte) {
		var asString = byte.toString(16).toUpperCase();
		while (asString.length < 2) {
			asString = '0' + asString;
		}

		return '0x' + asString;
	}

	this._init(source);
}
