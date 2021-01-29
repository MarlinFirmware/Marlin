var wmEnums = {
  Panels           : { STATUS:0, CONTROLS:1, FILES:2, CONSOLE:3 },
  ConsoleDirection : { APPEND:0, PREPEND:1 },
  ConsoleLevels    : { ERROR:0, SUCCESS:1, INFO:2, MONITOR:3, WARNING:4},
  WSMsgDirection   : { SENT:0, RECEIVED:1 },
  WsMsgSymbols     : { SENT:{LETTER:"S",ICON:null}, RECEIVED:{LETTER:"R",ICON:null} },
  WSSatuses        : { CONNECTING:0, OPEN:1, CLOSING:2, CLOSED:3 },
  LogLevels        : { INFO:0, WARNING:1, ERROR:2, DEBUG:3, VERBOSE:4},
  TempUnits        : { CELSIUS: {LABEL:"Celsius",VALUE:0,GP:"C"}, FAHRENHEIT:{LABEL:"Fahrenheit",VALUE:1,GP:"F"}, KELVIN:{LABEL:"Kelvin",VALUE:2,GP:"K"} }
};

var wmSettings = {
  AppName: "Marlin WebUI",
  AppRelease: "January, 1 2020",
  AppVersion: "1.1",
  AutoConnect: false,
  ConsoleDirection: wmEnums.ConsoleDirection.PREPEND,
  DefaultPanel: wmEnums.Panels.CONTROLS,
  LogLevel: wmEnums.LogLevels.VERBOSE,
  SymbolMode: 'letter',
  SymbolSend: wmEnums.WsMsgSymbols.SENT.LETTER,
  SymbolReceive: wmEnums.WsMsgSymbols.RECEIVED.LETTER,
  AutoTempInterval: 1,
  TempUnit: wmEnums.TempUnits.CELSIUS
};

class wmLogItem {
  constructor(text, mdir, mrs, gcmd=null, bgclass=null, ficon=null) {
    this.DateTime = wmTools.GetDateTime();
    this.Text = text;
    this.Direction = mdir;
    this.RsType = mrs;
    this.GCode = gcmd;
    this.BgClass = bgclass === null ? 'console-list-items-info': bgclass;
    this.FontIcon = ficon === null ? wmIcons.InfoCircle : ficon;
    this.SdFile = null;
  }
  SetValues(text=null, mdir=null, mrs=null, gcmd=null, bgclass=null, ficon=null) {
    if(text !== null) { this.Text = text; };
    if(mdir !== null) { this.Direction = mdir; };
    if(mrs !== null) { this.RsType = mrs; };
    if(gcmd !== null) { this.GCode = gcmd; };
    if(bgclass !== null) { this.BgClass = bgclass; };
    if(ficon !== null) { this.FontIcon = ficon; };
  }
  ToJson() { return JSON.stringify(this); }
  ToCsv() { return wmTools.StringFormatCsv(this); }
  ToString() { return wmTools.Stringfy(this); }
  ToLoglist(){
    switch (this.RsType) {
      case wmEnums.ConsoleLevels.INFO:
        this.BgClass = "console-list-items-info";
        this.FontIcon = wmIcons.InfoCircle;
        break;
      case wmEnums.ConsoleLevels.SUCCESS:
        this.BgClass = "console-list-items-success";
        this.FontIcon = wmIcons.CheckSquare;
        break;
      case wmEnums.ConsoleLevels.ERROR:
        this.BgClass = "console-list-items-error";
        this.FontIcon = wmIcons.Triangle;
        break;
      case wmEnums.ConsoleLevels.MONITOR:
        this.BgClass = "console-list-items-terminal";
        this.FontIcon = wmIcons.Terminal;
        break;
      case wmEnums.ConsoleLevels.WARNING:
        this.BgClass = "console-list-items-warning";
        this.FontIcon = wmIcons.Triangle;
        break;
    }
    let strout = '<li class="list-group-item console-list-items '+this.BgClass+'">';
    strout += '<span class="badge badge-light mr-1">'+this.FontIcon.ToHtml()+"</span>";
    strout += '<span class="badge badge-secondary mr-1">'+this.DateTime+'</span>';
    strout += '<span class="badge badge-'+(this.Direction === wmEnums.WSMsgDirection.SENT ? "danger" : "success")+' mr-1">';
    strout += this.Direction === wmEnums.WSMsgDirection.SENT ? wmSettings.SymbolSend : wmSettings.SymbolReceive;
    strout += '</span>' + this.Text + '</li>';
    return strout;
  }
  ToSdFileList() {
    if(this.SdFile !== "Begin file list" && this.SdFile !== "End file list") {
      let a = this.SdFile.split(" ");
      let strout = '<a href="javascript:void(0);" class="list-group-item list-group-item-action list-group-item-light p-1" data-sdfile="'+a[0]+'" onclick="WmButtons.SetSdSelected(this)">';
      strout += '<i class="icon icon-file mr-1"></i>'+a[0]+'<div class="badge badge-secondary float-right">'+wmTools.FileSizeFormat(a[1])+'</div>';
      strout += '</a>';
      return strout;
    }
  }
  static ParseWsMessage(msg) {
    let li = new wmLogItem();
    li.Direction = wmEnums.WSMsgDirection.RECEIVED;
    li.RsType = wmEnums.ConsoleLevels.SUCCESS;
    if (msg === "ok") {
      jsLog.Debug("WSMessage match: ok => " + msg);
      li.Text = "Acknowledge: "+msg;
    }
    else if (msg === "Not SD printing") {
      jsLog.Debug("WSMessage match: " + msg);
      li.Text = "Ack: "+msg;
      WmControls.SetPrinterStatusInfo(false, msg);
    }
    else if (msg.substring(0, 5) === "echo:") {
      if (msg.substring(5, 21) === "busy: processing") {
        jsLog.Debug("WSMessage match: echo:busy: processing: => " + msg);
        li.Text = msg.substring(5, msg.length);
        li.RsType = wmEnums.ConsoleLevels.WARNING;
      }
      else if (msg.substring(5, 21) === "Unknown command:") {
        jsLog.Debug("WSMessage match: echo:Unknown command: => " + msg);
        li.Text = msg.substring(5, msg.length);
        li.RsType = wmEnums.ConsoleLevels.WARNING;
      }
      else if (msg.substring(5,20) === "Now fresh file:") {
        jsLog.Debug("WSMessage match: echo:Now fresh file: => " + msg);
        li.Text = "SD: "+msg.substring(5, msg.length);
      }
      else if (msg==="File selected") {
        jsLog.Debug("WSMessage match: echo:File selected: => " + msg);
        li.Text = "SD: "+msg.substring(5, msg.length);
      }
    }
    else if (msg.substring(0, 6) === "Error:") {
      jsLog.Debug("WSMessage match: error => " + msg);
      li.Text = msg.substring(6, msg.length);
      li.RsType = wmEnums.ConsoleLevels.ERROR;
    }
    else if (msg.substring(0, 12) === "File opened:" || msg === "File selected") {
      jsLog.Debug("WSMessage match: File opened/selected => " + msg);
      li.Text = "SD: "+msg;
    }
    else if (msg.includes("open failed, File:")) {
      jsLog.Error("WSMessage match: open file error => " + msg);
      li.Text = "SD Error: "+msg;
      li.RsType = wmEnums.ConsoleLevels.ERROR;
    }
    else if (msg.toLowerCase().includes(".gco") || msg.toLowerCase().includes(".gcode") || msg.toLowerCase().includes(".g") || msg==="Begin file list" || msg==="End file list") {
      if(msg.substring(0,16)==="Writing to file:") { WmUpload.ReadyToWrite = true; }
      li.Text = "SD: "+msg;
      li.SdFile = msg;
    }
    //else if() {
    //}
    else {
      let rgx_rtemp_eb = /^T:\d{1,3}\.\d{1,2}\s+\/\d{1,3}\.\d{1,2}\s+B:\d{1,3}\.\d{1,2}\s+\/\d{1,3}\.\d{1,2}/;
      let rgx_rtemp_e  = /^T:\d{1,3}\.\d{1,2}\s+\/\d{1,3}\.\d{1,2}/;

      if(rgx_rtemp_eb.test(msg) || rgx_rtemp_e.test(msg)){
        jsLog.Verbose("Extruder temperatore report: "+msg);
        msg = msg.replace(/\//g,"");
        let tarr = msg.split(/\s/);
        WmCharts.SetTempReport(tarr);
        li.Text = "Temp report: "+msg;
      }
      else { li.Text = msg; }
    }
    jsLog.Verbose(li.ToString());
    return li;
  }
}

class wmGCommandItem {
  constructor(g,p,v,d,s=null) {
    this.GCode = g;
    this.GParams = p;
    this.Values = v;
    this.Description = d;
    this.Supported = s===null ? true : s;
  }
  ToJson() { return JSON.stringify(this); }
  ToString() { return wmTools.Stringfy(this); }
  static CalcChecksum(gc) {
    let cs = 0;
    gc = gc.toUpperCase().replace(/\s/g, '');
    for(let i=0; gc[i]!=='*' && gc[i]!==null && i<gc.length; i++) { cs = cs ^ gc.charCodeAt(i); }
    jsLog.Verbose("Calculate GCommand checksum of: "+gc+" => "+cs);
    return cs;
  }
  static GetCommandItemByCode(gc) {
    jsLog.Verbose("GetCommandItemByCode: Find preset for: "+gc);
    let sgc = gc.split(/\s/);
    jsLog.Verbose("GetCommandItemByCode: Command to find: "+sgc[0]);
    for (let [k, v] of Object.entries(wmGCommands)) {
      if(v.GCode.indexOf(sgc[0]) > -1) { return v; }
    }
    return null;
  }
}

class wmFontIcon {
  constructor(ico, mc = null) {
    this.ico = ico;
    this.mClass = mc !== null ? " "+mc : "";
  }
  ToString() { return wmTools.Stringfy(this); }
  ToHtml() { return "<i class=\"icon icon-"+ this.ico + this.mClass+"\"></i>"; }
  AddClass(acl) { return "<i class=\"icon icon-"+ this.ico + " "+ acl+"\"></i>"; }
}

class wmTools {
  static Stringfy(obj) {
    if(obj !== null && obj !== "undefined") {
      let rt = "";
      for (let [k, v] of Object.entries(obj)) { rt += `${k}=${v};`; }
      return rt;
    } else {
      return obj;
    }
  }
  static StringFormat() {
    var args = Array.prototype.slice.call(arguments, 1);
    return arguments[0].replace(/\{(\d+)\}/g, function (match, index) { return args[index]; });
  }
  static StringFormatJson(str) {
    return JSON.stringify(str);
  }
  static StringFormatCsv(obj) {
    let rt = "";
    for (let [k, v] of Object.entries(obj)) { rt += `${v},`; }
    return rt;
  }
  static StringRemoveSpecials(str) {
    var spc = [".","~","{","}"];
    for (var i=0; i<spc.length; i++) { str = str.replace(spc[i],""); }
    return str;
  }
  static FileDownload(fname, ftype, fdata){
    var blob = new Blob(fdata, {type: ftype});
    window.saveAs(blob, fname);
  }
  static FileSizeFormat(size) {
    if(size>0) {
      var i = Math.floor( Math.log(size) / Math.log(1000) );
      return ( size / Math.pow(1000, i) ).toFixed(2) * 1 + ' ' + ['B', 'kB', 'MB', 'GB', 'TB'][i];
    } else {
      return size+" B";
    }
  }
  static EscapeHtml(unsafe) {
    unsafe = unsafe.replace(/&/g, "&amp;");
    unsafe = unsafe.replace(/</g, "&lt;");
    unsafe = unsafe.replace(/>/g, "&gt;");
    unsafe = unsafe.replace(/"/g, "&quot;");
    unsafe = unsafe.replace(/'/g, "&#039;");
    return unsafe;
  }
  static GetDateTime() {
    var dt = new Date();
    var hr = dt.getHours() < 10 ? "0" + dt.getHours() : dt.getHours();
    var mn = dt.getMinutes() < 10 ? "0" + dt.getMinutes() : dt.getMinutes();
    var sc = dt.getSeconds() < 10 ? "0" + dt.getSeconds() : dt.getSeconds();
    return hr + ":" + mn + ":" + sc;
  }
  static GetBrowser() {
    return $.browser.name+" v"+$.browser.versionNumber+" on "+$.browser.platform;
  }
  static GetScreenSize() {
    let bwsize = "Viewport="+$(window).width()+"x"+$(window).height();
    bwsize += " Document="+$(document).width()+"x"+$(document).height();
    bwsize += " Screen="+window.screen.width+"x"+window.screen.height;
    return bwsize;
  }
  static GetNumPercent(p,n) {
     return (p / 100) * n;
  }
  static GetPercentage(p,n) {
     return (p * 100) / n;
  }
  static FormatNumber(v,d) {
    v = parseFloat(v);
    return v.toFixed(d).toString().replace(/(\d)(?=(\d{3})+(?!\d))/g, '$1.');
  }
  static CelsiusToFahrenheit(n) {
    return n * 9 / 5 + 32;
  }
  static CelsiusToKelvin(n) {
    return n+273.15;
  }
}

class wmCookie {
  static Read(cname) {
    let decCookie = decodeURIComponent(document.cookie);
    let carr = decCookie.split(';');
    for (let i=0; i<carr.length; i++) {
      while (carr[i].charAt(0)===' ') { carr[i] = carr[i].substring(1); }
      if (carr[i].indexOf(cname)===0) {
        let r = carr[i].substring(cname.length+1, carr[i].length);
        jsLog.Verbose("Load cookie '"+cname+"' => "+r);
        return r;
      }
    }
    return null;
  }
  static Write(cvalue, cname=null) {
    let d = new Date();
    d.setTime(d.getTime() + (365 * 24 * 60 * 60 * 1000));
    let expires = "expires=" + d.toGMTString();
    let cn = cname === null ? "E4dWUI" : cname;
    let cv = cn+ "=" + escape(cvalue) + "; " + expires + ";path=/; SameSite=None; Secure;";
    document.cookie = cv;
    jsLog.Verbose("Add/Update cookie => "+cv);
  }
  static CheckBrowser() {
    wmCookie.Write('1','check_browser_cookie');
    return (document.cookie.indexOf('check_browser_cookie') !== -1) ? true : false;
  }
  static Check() {
    jsLog.Verbose("Checking for browser supported cookie");
    if(wmCookie.CheckBrowser()===true) {
      jsLog.Debug("Cookies supported. Looking for custom settings");
      var cStr = wmCookie.Read("E4dWUI");
      if(cStr === null){
        jsLog.Verbose("No settings cookie found. Define defaults");
        wmCookie.Write(JSON.stringify(wmSettings));
      } else {
        jsLog.Debug("Settings cookie found. Loading customized settings");
        var cv = JSON.parse(cStr);
        wmSettings.AutoConnect = cv.AutoConnect;
        wmSettings.DefaultPanel = cv.DefaultPanel;
        wmSettings.ConsoleDirection = cv.ConsoleDirection;
        wmSettings.LogLevel = cv.LogLevel;
        wmSettings.SymbolMode = cv.SymbolMode,
        jsLog.Verbose("Customized cookie stored settings loaded");
        jsLog.Verbose(wmTools.Stringfy(wmSettings));
      }
    } else {
      jsLog.Warning("Cookies are not supported by the browser. Use default settings");
    }
  }
}

class jsLog {
  static Info(logmsg) {
    if(wmSettings.LogLevel >= wmEnums.LogLevels.INFO) { console.log("[INFO   ] "+logmsg); }
  }
  static Warning(logmsg) {
    if(wmSettings.LogLevel >= wmEnums.LogLevels.WARNING) { console.log("[WARN   ] "+logmsg); }
  }
  static Error(logmsg) {
    if(wmSettings.LogLevel >= wmEnums.LogLevels.ERROR)   { console.log("[ERROR  ] "+logmsg); }
  }
  static Debug(logmsg) {
    if(wmSettings.LogLevel >= wmEnums.LogLevels.DEBUG)   { console.log("[DEBUG  ] "+logmsg); }
  }
  static Verbose(logmsg) {
    if(wmSettings.LogLevel >= wmEnums.LogLevels.VERBOSE) { console.log("[VERBOSE] "+logmsg); }
  }
}

var wmGCommands = {
  CustomCmd     : new wmGCommandItem('',null,null,'Custom command'),
  MoveFw        : new wmGCommandItem('G1','Y{0}',10,'Move forward on Y axis'),
  MoveBw        : new wmGCommandItem('G1','Y-{0}',10,'Move backward on Y axis'),
  MoveSx        : new wmGCommandItem('G1','X{0}',10,'Move left on X axis'),
  MoveDx        : new wmGCommandItem('G1','X-{0}',10,'Move right on X axis'),
  MoveUp        : new wmGCommandItem('G1','Z{0}',10,'Move up on Z axis'),
  MoveDw        : new wmGCommandItem('G1','Z-{0}',10,'Move down on Z axis'),
  FillRetrive   : new wmGCommandItem('G10',null,null,'Retract filament'),
  FillExtrude   : new wmGCommandItem('GYYYY',null,null,'Extrude filament'),
  MoveHome      : new wmGCommandItem('G28',null,null,'Go home on all axis'),
  MoveHomeX     : new wmGCommandItem('G28','X',null,'Go home on X axis'),
  MoveHomeY     : new wmGCommandItem('G28','Y',null,'Go home on Y axis'),
  MoveHomeZ     : new wmGCommandItem('G28','Z',null,'Go home on Z axis'),
  StepEnable    : new wmGCommandItem('M17','{0}','E X Y Z','Enable stepper'),
  StepEnableAll : new wmGCommandItem('M17',null,null,'Enable all steppers'),
  StepDisable   : new wmGCommandItem('M18','{0}','E X Y Z','Disable stepper'),
  StepDisableAll: new wmGCommandItem('M18',null,null,'Disable all steppers'),
  SdGetList     : new wmGCommandItem('M20',null,null,'Get SD card content'),
  SdInit        : new wmGCommandItem('M21',null,null,'Init SD card'),
  SdRelease     : new wmGCommandItem('M22',null,null,'Release SD card'),
  SdFileSel     : new wmGCommandItem('M23','{0}','','Select an SD file'),
  SdFilePrint   : new wmGCommandItem('M24','{0}','','Start an SD print'),
  SdPrintStatus : new wmGCommandItem('M27',null,null,'SD print status'),
  SdPrintReport : new wmGCommandItem('M27','S{0}',5,'SD print status report'),
  SdFileStart   : new wmGCommandItem('M28','{0}','','Start SD write'),
  SdFileStop    : new wmGCommandItem('M29',null,null,'Stop SD write'),
  SdFileDel     : new wmGCommandItem('M30','{0}','','Delete an SD file'),
  PrintTime     : new wmGCommandItem('M31',null,null,'Print time'),
  FanOn         : new wmGCommandItem('M106','S{0}',128,'Set fan on with speed'),
  FanOff        : new wmGCommandItem('M107',null,null,'Set fan off'),
  GetPosition   : new wmGCommandItem('M114',null,null,'Get Current Position'),
  FWInfo        : new wmGCommandItem('M115',null,null,'Get firmware info',false),
  SetTempUnit   : new wmGCommandItem('M149','{0}','C','Set temperature units'),
  SetTempOff    : new wmGCommandItem('M155','S0','','Turn off temperature status'),
  SetTempOn     : new wmGCommandItem('M155','S{0}',1,'Get temp status (1 sec default)'),
  GetSetting    : new wmGCommandItem('M503',null,null,'Get settings report')
};

var wmIcons = {
  Wifi: new wmFontIcon('wifi'),
  Plug: new wmFontIcon('plug'),
  Ban: new wmFontIcon('ban'),
  Bolt: new wmFontIcon('bolt'),
  Info: new wmFontIcon('info'),
  InfoCircle: new wmFontIcon('info-circled'),
  Triangle: new wmFontIcon('exclamation-triangle'),
  CheckSquare: new wmFontIcon('check-square'),
  Terminal: new wmFontIcon('terminal'),
  Exchange: new wmFontIcon('exchange'),
  ChevronUp: new wmFontIcon('chevron-up'),
  ChevronDown: new wmFontIcon('chevron-down'),
  ChevronLeft: new wmFontIcon('chevron-left'),
  ChevronRight: new wmFontIcon('chevron-right'),
  LongArrowUp: new wmFontIcon('long-arrow-up'),
  LongArrowDown: new wmFontIcon('long-arrow-down'),
  LongArrowLeft: new wmFontIcon('long-arrow-left'),
  LongArrowRight: new wmFontIcon('long-arrow-right')
};

var wmColors = {
  Black   : 'rgb(0, 0, 0)',
  Blue    : 'rgb(54, 162, 235)',
  Green   : 'rgb(0, 255, 0)',
  GreenSuc: 'rgb(92, 184, 92)',
  GreenTur: 'rgb(75, 192, 192)',
  Grey    : 'rgb(201, 203, 207)',
  Yellow  : 'rgb(255, 205, 86)',
  Orange  : 'rgb(255, 159, 64)',
  Purple  : 'rgb(153, 102, 255)',
  Red     : 'rgb(255, 0, 0)',
  RedCoral: 'rgb(255, 99, 132)'
};
// Define default setting onject
jsLog.Verbose("JS Classes initializzation completed");
jsLog.Debug("Default settings loaded: "+wmTools.Stringfy(wmSettings));
wmCookie.Check();
