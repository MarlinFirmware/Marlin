var wmLogBuffer = new Array();
var wmSdListCounter = {
  FILES:0,
  FOLDERS:0,
  Reset: function() {
    wmSdListCounter.FILES = 0;
    wmSdListCounter.FOLDERS = 0;
    $('#list-sd-content').empty();
    $('#div-sdlist-file-count').text(wmSdListCounter.FILES);
    $('#div-sdlist-folder-count').text(wmSdListCounter.FOLDERS);
  }
};

var WmButtonGroups = {
  FileManagement: ["#btn-get-sdcontent","#btn-set-sdinit","#btn-set-sdrelease","#file-upload","#btn-file-upload"],
  FileActions: ["#btn-set-sdprint","#btn-set-sddelete"],
  FileProcess: ["#btn-file-proc","#btn-file-proc-cancel"],
  TempStatus: ["#set-auto-temp","#auto-temp-interval","#chart-show-extruder","#chart-show-bed"],
  FanSpeed: ["#fan-speed-range"],
  MoveHome: ["#btn-move-home-all","#btn-move-home-x","#btn-move-home-y","#btn-move-home-z"],
  Move: ["#btn-move-xl","#btn-move-xr","#btn-move-yf","#btn-move-yb","#btn-move-zu","#btn-move-zd"],
  StepperAll: ["#set-stepper-all","#set-stepper-x","#set-stepper-y","#set-stepper-z","#set-stepper-e"],
  Stepper: ["#set-stepper-x","#set-stepper-y","#set-stepper-z","#set-stepper-e"],

  All: function() {
    let all = [];
    all = all.concat(
      WmButtonGroups.FileManagement,
      WmButtonGroups.FileActions,
      WmButtonGroups.FileProcess,
      WmButtonGroups.TempStatus,
      WmButtonGroups.FanSpeed,
      WmButtonGroups.MoveHome,
      WmButtonGroups.Move,
      WmButtonGroups.StepperAll
    );
    return all;
  }
};

var wmWebSoket = {
  WSObject: null,
  Connect: function() {
    WsUrl=`ws://${location.host}/ws`;
    try {
      if(wmWebSoket.WSObject === null) {
        jsLog.Debug("WebSocket: Trying connecting to " + WsUrl);
        wmWebSoket.WSObject = new WebSocket(WsUrl);
        wmWebSoket.SetWsStatusBar(wmWebSoket.WSObject.readyState);
        wmWebSoket.WSObject.onopen = function () {
          jsLog.Info("WebSocket: Successfully connected to " + WsUrl);
          wmWebSoket.SetWsStatusBar(wmWebSoket.WSObject.readyState);
          WmControls.Enable(WmButtonGroups.All());
          wmWebSoket.Send(wmGCommands.SdPrintStatus);
        };
        wmWebSoket.WSObject.onclose = function () {
          jsLog.Info("WebSocket: Disconnected from "+WsUrl);
          wmWebSoket.SetWsStatusBar(null);
          wmWebSoket.WSObject = null;
          WmControls.Disable(WmButtonGroups.All());
        };
        wmWebSoket.WSObject.onerror = function () {
          jsLog.Error("WebSocket: Connection error");
          WmConsole.Trace(new wmLogItem("WebSoket connection error", wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR));
        };
        wmWebSoket.WSObject.onmessage = function (event) {
          jsLog.Info("WebSocket: Message received: "+event.data);
          wmWebSoket.OnMessage(event.data.trim());
        };
      }
    }
    catch (exception) {
      jsLog.Error("WebSocket: Exception: "+exception);
      wmWebSoket.SetWsStatusBar(wmWebSoket.WSObject.readyState);
      WmConsole.Trace(new wmLogItem("WebSocket: Connection exception", wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR));
    }
  },
  Disconnect: function() {
    try {
      if(wmWebSoket.WSObject !== null && wmWebSoket.WSObject.readyState === wmEnums.WSSatuses.OPEN) {
        jsLog.Debug("WebSocket: Disconnecting from "+WsUrl);
        WmConsole.Trace(new wmLogItem("WebSoket disconnecting...", wmEnums.WSMsgDirection.SENT, wmEnums.ConsoleLevels.INFO));
        wmWebSoket.WSObject.close();
        wmWebSoket.WSObject = null;
      }
    }
    catch (exception) {
      jsLog.Error("WebSocket: Exception: "+exception);
      wmWebSoket.SetWsStatusBar(wmWebSoket.WSObject.readyState);
      WmConsole.Trace(new wmLogItem("WebSoket connection exception", wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR));
    }
  },
  Send: function(gCmd) {
    if(wmWebSoket.WSObject !== null && wmWebSoket.WSObject.readyState === wmEnums.WSSatuses.OPEN) {
      let strcmd = gCmd.GCode;
      if (gCmd.GParams === null) { jsLog.Verbose('WebSocket: Send: Command with no params detected'); }
      else {
        jsLog.Verbose('WebSocket: Send: Arguments detected:'+gCmd.GParams);
        gCmd.GParams = wmTools.StringFormat(gCmd.GParams, gCmd.Value);
        strcmd = strcmd +" "+gCmd.GParams;
      }
      jsLog.Debug('WebSocket: Send commandstring: '+strcmd);
      WmConsole.Trace(new wmLogItem("GCmd: <span class=\"badge badge-light\">" + strcmd + "</span> " + gCmd.Description, wmEnums.WSMsgDirection.SENT, wmEnums.ConsoleLevels.SUCCESS));
      try { wmWebSoket.WSObject.send(strcmd + '\n'); }
      catch (exception) {
        jsLog.Error('WebSocket: Exception:' + exception);
        WmConsole.Trace(new wmLogItem("WebSoket: Command exception: "+exception, wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR));
      }
    } else { $('#modal-connect').modal('show'); }
  },
  OnMessage: function(mdt) {
    if(mdt === "") { jsLog.Debug("WSMessage match: Empty message (skipped)"); }
    else {
      let litem = wmLogItem.ParseWsMessage(mdt);
      WmConsole.Trace(litem);
      if(litem.SdFile !== null) { WmConsole.TraceSdFile(litem); }
    }
  },
  SetWsStatusBar: function(rs) {
    let cli = new wmLogItem();
    let ctrls = { div:$('#div-conn-statusmsg'), ico:$('#div-conn-statusico'), btn:$('#btn-connect-status'), bar:$('#div-conn-statusbar') };
    switch (rs) {
      case wmEnums.WSSatuses.CONNECTING:
        cli.SetValues("Connecting to "+WsUrl, wmEnums.WSMsgDirection.SENT, wmEnums.ConsoleLevels.INFO);
        ctrls.ico.html("<span class=\"blink\">"+wmIcons.Exchange.AddClass('')+"</span>");
        ctrls.div.html("Connecting...");
        ctrls.btn.html("<span class=\"spinner-border spinner-border-sm\"></span>");
        ctrls.bar.removeClass('bg-success bg-warning bg-danger text-light text-dark').addClass("bg-warning text-dark");
        break;
      case wmEnums.WSSatuses.OPEN:
        cli.SetValues("Connected to "+WsUrl, wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.SUCCESS);
        ctrls.ico.html(wmIcons.Wifi.AddClass(''));
        ctrls.div.html("Connected");
        ctrls.btn.html(wmIcons.Ban.ToHtml());
        ctrls.bar.removeClass('bg-success bg-warning bg-danger text-light text-dark').addClass("bg-success text-light");
        break;
      case wmEnums.WSSatuses.CLOSED:
        cli.SetValues("Disconnected from "+WsUrl, wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR);
        ctrls.ico.html(wmIcons.Plug.AddClass(''));
        ctrls.div.html("Disconnected");
        ctrls.btn.html(wmIcons.Bolt.ToHtml());
        ctrls.bar.removeClass('bg-success bg-warning bg-danger text-light text-dark').addClass("bg-danger text-light");
        break;
      case wmEnums.WSSatuses.CLOSING:
        cli.SetValues("Disconnecting from "+WsUrl, wmEnums.WSMsgDirection.SENT, wmEnums.ConsoleLevels.WARNING);
        ctrls.ico.html("<span class=\"blink\">"+wmIcons.Wifi.AddClass('')+"</span>");
        ctrls.div.html("Disconnecting...");
        ctrls.btn.html("<span class=\"spinner-border spinner-border-sm\"></span>");
        ctrls.bar.removeClass('bg-success bg-warning bg-danger text-light text-dark').addClass("bg-warning text-dark");
        break;
      default:
        cli.SetValues("Disconnected from "+WsUrl, wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR);
        ctrls.ico.html(wmIcons.Plug.AddClass(''));
        ctrls.div.html("Disconnected");
        ctrls.btn.html(wmIcons.Bolt.ToHtml());
        ctrls.bar.removeClass('bg-success bg-warning bg-danger text-light text-dark').addClass("bg-danger text-light");
        break;
    }
    WmConsole.Trace(cli);
  }
};

var WmUpload = {
  Reader: null,
  FileName: null,
  FileSize: null,
  FileContent: null,
  ReadyToWrite: false,
  Cancelled: false,
  Load: function() {
    let [fn,fe] = $("#file-upload-label").text().toUpperCase().split(".");
    WmUpload.FileName = fn.substring(0,7)+".GCO";
    let input = $('#file-upload').get(0);
    if (input.files.length) {
      let tfile = input.files[0];
      WmUpload.Reader = new FileReader();
      WmUpload.Reader.onloadstart = function() {
        jsLog.Debug("File uploading starting");
        $('#div-upload-fname').html(WmUpload.FileName);
        if(!$('#upload-process-collapse').hasClass("show")){ $('#upload-process-collapse').collapse('show'); }
      };
      WmUpload.Reader.onload = function(e) {
        jsLog.Debug("File uploading completed");
        WmUpload.FileSize = e.loaded;
        WmUpload.FileContent = e.target.result.split("\n");
        WmUpload.Cancelled = false;
        $('#div-upload-fsize').html(wmTools.FileSizeFormat(WmUpload.FileSize));
        $('#div-upload-fproc').html(wmTools.FormatNumber(WmUpload.FileContent.length,0));
        WmControls.Enable(WmButtonGroups.FileProcess);
        WmUpload.FileProgress(0,"Ready to process...");

      };
      WmUpload.Reader.onloadend = function(e) {
        jsLog.Debug("File uploading finished");
        jsLog.Debug("___________________onloadend");
      };
      WmUpload.Reader.readAsText(tfile);
      return true;
    } else {
      WmControls.ShowModalAlert('Please select the upload file before continuing');
      WmControls.Enable(WmButtonGroups.FileManagement,WmButtonGroups.FileActions);
      return false;
    }
  },
  Cancel: function() {
    jsLog.Debug("File uploading aborted");
    WmUpload.Reader = null;
    WmUpload.FileName = null;
    WmUpload.FileSize = null;
    WmUpload.Cancelled = true;
    WmControls.Disable(WmButtonGroups.FileProcess);
    WmControls.Enable(WmButtonGroups.FileManagement,WmButtonGroups.FileActions);
    $('#div-upload-fsize').html("-");
    $('#div-upload-fproc').html("-");
    $('#div-upload-fname').html("-");
    WmUpload.FileProgress(0,"&nbsp;");
    if($('#upload-process-collapse').hasClass("show")){ $('#upload-process-collapse').collapse('hide'); }
  },
  FileProcess: function() {
    WmControls.Disable([$("#btn-file-proc")]);
    let fl = WmUpload.FileContent.length;
    jsLog.Debug("Start process GCode lines ("+WmUpload.FileSize+" total)");
    WmUpload.FileProgress(0,"Start analyzing uploaded GCode...");
    wmGCommands.SdFileStart.GParams = WmUpload.FileName;
    wmWebSoket.Send(wmGCommands.SdFileStart);
    var i = 0;
    var n = 1;
    (function pgline() {
      if(WmUpload.Cancelled){
        wmWebSoket.Send(wmGCommands.SdFileStop);
        return;
      }
      else if(!WmUpload.ReadyToWrite){
        jsLog.Debug("WmUpload.FileProcess: Waiting ready to write...");
        WmUpload.FileProgress(0,"Waiting ready to write...");
        setTimeout(pgline, 500);
      }
      else {
        let p = wmTools.GetPercentage(i+1,fl);
        WmUpload.FileProgress(p,"Analyzing line "+(i+1)+" of "+fl);
        let gitem = { line:WmUpload.FileContent[i], process:false, cksum:0 };

        if(gitem.line.trim()==="" || gitem.line.match(/^ *$/)) { jsLog.Verbose("GLine: "+i+": "+gitem.line+" => Empty line (skip)"); }
        else if(gitem.line.substring(0,1)===";") { jsLog.Verbose("GLine: "+i+": "+gitem.line+" => Comment line (skip)"); }
        else if(gitem.line.indexOf(";") > -1) { gitem.line = gitem.line.substring(0,gitem.line.indexOf(";")); gitem.process=true; }
        else { gitem.process = true; }

        if(gitem.process) {
          gitem.line = "N"+n+" "+gitem.line.trim();
          gitem.line = gitem.line+"*"+wmGCommandItem.CalcChecksum(gitem.line);
          jsLog.Verbose("GLINE TO SEND: "+gitem.line);
          wmGCommands.CustomCmd.GCode = gitem.line;
          wmWebSoket.Send(wmGCommands.CustomCmd);
          n++;
        }
        i++;
        if (i < fl) { setTimeout(pgline, 10); }
        else {
          WmUpload.FileProgress(100,"GCode Analysis completed!");
          WmUpload.ReadyToWrite = false;
          wmWebSoket.Send(wmGCommands.SdFileStop);
          WmUpload.FileCompleted();
        }
      }
    })();
  },
  FileProgress: function(p,m) {
    p = wmTools.FormatNumber(p,0);
    $('#upload-progressbar').text(p+"%");
    $('#upload-progressbar').css('width',p+'%').attr('aria-valuenow', p);
    $('#upload-progress-text').html(m);
    if(p===100) { $('#upload-progressbar').removeClass("progress-bar-animated"); }
  },
  FileCompleted: function() {
    WmControls.Disable(WmButtonGroups.FileProcess);
    WmButtons.GetSdContentList();
  },
};

var WmButtons = {
  ConsoleListClear: function() { WmConsole.Clear(); },
  ConsoleListExport: function() { WmConsole.Export(); },
  DeleteSdConfirm: function() {
    $('#div-sdfile-delete-badge').html($('#txt-sdfile-selected').val());
  },
  DeleteSdSelected: function() {
    jsLog.Debug("DeleteSdSelected: Delete file:"+$("#txt-sdfile-selected").val());
    WmControls.Disable(WmButtonGroups.FileActions);
    $('#div-sdfile-delete-rs').collapse('show');
    wmGCommands.SdFileDel.GParams = $("#txt-sdfile-selected").val();
    wmWebSoket.Send(wmGCommands.SdFileDel);
    setTimeout(function(){
      $('#modal-sdfile-delete').modal('hide');
      $('#div-sdfile-delete-rs').collapse('hide');
      WmButtons.GetSdContentList();
    }, 2000);
  },
  GCommandSetPreset: function(gc) {
    jsLog.Debug("Set preset GCommand ("+gc+")");
    $('#text-gcommand').val(gc);
    $('#modal-presets').modal('hide');
    WmAutostart.SetGCommandChecksum();
  },
  GetSdContentList: function() {
    wmSdListCounter.Reset();
    $('#txt-sdfile-selected').val('');
    WmControls.Disable(WmButtonGroups.FileManagement);
    wmWebSoket.Send(wmGCommands.SdGetList);
  },
  PrintSdConfirm: function() {
    $('#div-sdfile-print-badge').html($('#txt-sdfile-selected').val());
  },
  PrintSdSelected: function() {
    jsLog.Debug("PrintSdSelected: Print file:"+$("#txt-sdfile-selected").val());
    WmControls.Disable(WmButtonGroups.FileActions);
    $('#div-sdfile-print-rs').collapse('show');
    wmGCommands.SdFilePrint.GParams = $("#txt-sdfile-selected").val();
    wmWebSoket.Send(wmGCommands.SdFilePrint);
    setTimeout(function(){
      $('#modal-sdfile-print').modal('hide');
      $('#div-sdfile-print-rs').collapse('hide');
      WmAutostart.SetShownPanel(wmEnums.Panels.STATUS);
    }, 2000);
  },
  SaveSettings: function() {
    jsLog.Verbose("Button 'btn-save-settings' clicked");
    WmControls.Disable(['#btn-save-settings','#btn-close-settings']);
    wmSettings.AutoConnect = document.getElementById('set-auto-connect').checked;
    wmSettings.DefaultPanel = parseInt($('#set-default-panel').val());
    wmSettings.LogLevel = parseInt($('#set-log-level').val());
    wmSettings.SymbolMode = $('#set-log-symbol').val();
    wmSettings.AutoTempInterval = $('#set-default-autotemp').val();
    if($('#set-default-tempunit').val()===0) { wmSettings.TempUnit = wmEnums.TempUnits.CELSIUS; }
    else if($('#set-default-tempunit').val()===1) { wmSettings.TempUnit = wmEnums.TempUnits.FAHRENHEIT; }
    else if( $('#set-default-tempunit').val()===2) { wmSettings.TempUnit = wmEnums.TempUnits.KELVIN; }

    if(wmSettings.SymbolMode==='letter') {
      wmSettings.SymbolSend = wmEnums.WsMsgSymbols.SENT.LETTER;
      wmSettings.SymbolReceive = wmEnums.WsMsgSymbols.RECEIVED.LETTER;
    } else {
      wmSettings.SymbolSend = $('#div-log-symbol-icon-sample-s').html();
      wmSettings.SymbolReceive = $('#div-log-symbol-icon-sample-r').html();
    }
    if(document.getElementById('set-trace-mode-append').checked) {
      wmSettings.ConsoleDirection = wmEnums.ConsoleDirection.APPEND;
    } else {
      wmSettings.ConsoleDirection = wmEnums.ConsoleDirection.PREPEND;
    }
    wmCookie.Write(wmTools.StringFormatJson(wmSettings));
    $('#div-save-setting-rs').collapse('show');
    setTimeout(function(){
      $('#modal-settings').modal('hide');
      $('#div-save-setting-rs').collapse('hide');
      WmControls.Enable(['#btn-save-settings','#btn-close-settings']);
    }, 2000);
  },
  SdInit: function() {
    wmWebSoket.Send(wmGCommands.SdInit);
  },
  SdRelease: function() {
    wmWebSoket.Send(wmGCommands.SdRelease);
  },
  SendGcommand: function() {
    if(wmWebSoket.WSObject !== null && wmWebSoket.WSObject.readyState === wmEnums.WSSatuses.OPEN) {
      WmControls.Disable(["#btn-gcommand"]);
      let gcmd = $('#text-gcommand');
      if (gcmd.val() === "") {
        gcmd.removeClass('border-dark').addClass('border-danger');
        jsLog.Warning("Empty custom command string detected");
        gcmd.focus();
      } else {
        gcmd.removeClass('border-danger').addClass('border-dark');
        let gc = wmGCommandItem.GetCommandItemByCode(gcmd.val().trim().toUpperCase());
        if(gc === null) {
          wmGCommands.CustomCmd.GCode = gcmd.val().trim().toUpperCase();
          jsLog.Debug("Sending custom command: " + wmGCommands.CustomCmd.GCode);
          wmWebSoket.Send(wmGCommands.CustomCmd);
        } else {
          if(gc.Supported) {
            wmGCommands.CustomCmd.GCode = gcmd.val().trim().toUpperCase();
            jsLog.Debug("Sending custom command: " + wmGCommands.CustomCmd.GCode);
            wmWebSoket.Send(wmGCommands.CustomCmd);
          } else {
            jsLog.Warning("Unsupported command: " + wmGCommands.CustomCmd.GCode);
            WmConsole.Trace(new wmLogItem("GCmd: <span class=\"badge badge-light\">" + gcmd.val() + "</span> Unsupported command", wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.ERROR));
          }
        }
        gcmd.val('');
        $('#checksum-gcommand-value').html('&nbsp;');
        WmControls.Enable(["#btn-gcommand"]);
      }
    } else { $('#modal-connect').modal('show'); }
  },
  SetPositionHome: function(b) {
    if(b.id==="btn-move-home-all"){ wmWebSoket.Send(wmGCommands.MoveHome); }
    else if (b.id==="btn-move-home-x"){ wmWebSoket.Send(wmGCommands.MoveHomeX); }
    else if (b.id==="btn-move-home-y"){ wmWebSoket.Send(wmGCommands.MoveHomeY);}
    else if (b.id==="btn-move-home-z"){ wmWebSoket.Send(wmGCommands.MoveHomeZ);}
  },
  SetSdSelected: function(sdi) {
    jsLog.Debug("SetSdSelected: Selected file:"+$(sdi).attr("data-sdfile"));
    $('#txt-sdfile-selected').val($(sdi).attr("data-sdfile"));
    let lip = document.getElementById("list-sd-content").getElementsByTagName("a");
    for (let i=0; i<lip.length; i++) { $(lip[i]).removeClass('list-group-item-success').addClass("list-group-item-light"); }
    $(sdi).removeClass('list-group-item-light').addClass("list-group-item-success");
    WmControls.Enable(WmButtonGroups.FileActions);
    if(!$('#div-sd-selected-file').hasClass("show")) { $('#div-sd-selected-file').collapse("show"); }
    wmGCommands.SdFileSel.GParams = $(sdi).attr("data-sdfile");
    wmWebSoket.Send(wmGCommands.SdFileSel);
  },
  ToggleChecksumDiv: function() {
    let csdiv = $('#checksum-gcommand-div');
    if($('#btn-gcommand-checksum').hasClass('active')===true) { csdiv.collapse('hide'); } else { csdiv.collapse('show'); }
  },
  UploadSdFile: function() {
    WmControls.Disable(WmButtonGroups.FileManagement);
    WmControls.Disable(WmButtonGroups.FileActions);
    if (!window.FileReader) {
        jsLog.Error('Your browser do not support JS file uploading');
        alert('Your browser do not support JS file uploading');
        WmControls.Enable(WmButtonGroups.FileManagement,WmButtonGroups.FileActions);
    } else {
      jsLog.Debug("Starting upload file process");
      if(WmUpload.Load()) { jsLog.Debug("Upload completed"); }
      else {
        jsLog.Error("Upload failed");
        WmControls.Enable(WmButtonGroups.FileManagement,WmButtonGroups.FileActions);
      }
    }
  },
  WsConnect: function() {
    if(wmWebSoket.WSObject === null) { wmWebSoket.Connect(); }
    else { if(wmWebSoket.WSObject.readyState === wmEnums.WSSatuses.OPEN) { wmWebSoket.Disconnect(); } }
  },
};

var WmControls = {
  Enable: function() {
    if(arguments.length > 0) {
      for(let i=0; i<arguments.length; i++) {
        let fld = arguments[i];
        for(let c=0; c<fld.length; c++) { WmControls.SetInputStatus(fld[c],'enabled'); }
      }
    } else { jsLog.Warning("WmControls.Enable: Missing input arguments"); }
  },
  Disable: function() {
    if(arguments.length > 0) {
      for(let i=0; i<arguments.length; i++) {
        let fld = arguments[i];
        for(let c=0; c<fld.length; c++) { WmControls.SetInputStatus(fld[c],'disabled'); }
      }
    } else { jsLog.Warning("WmControls.Disable: Missing input arguments"); }
  },
  SetInputStatus(inid,st) {
    if($(inid).attr("data-input-type")==="togglebtn"){
      if(st==="enabled") { $(inid).bootstrapToggle('enable'); } else { $(inid).bootstrapToggle('disable'); }
    } else {
      if(st==="enabled") { $(inid).prop("disabled", false); } else { $(inid).prop("disabled", true); }
    }
  },
  SetCheckStatus: function(ao,b) {
    let cs="off";
    if(b) { cs="on"; }
    for(let i=0; i<ao.length; i++) { $(ao[i]).bootstrapToggle(cs); }
  },
  SetUpload: function() {
    let f = $("#file-upload").val();
    jsLog.Debug("SetUploadFilename: Ready to upload file:"+f);
    $("#file-upload-label").html(f.split("\\").pop());
    WmControls.Enable(WmButtonGroups.FileManagement);
    WmControls.Disable(WmButtonGroups.FileProcess);
  },
  ShowModalAlert: function(m) {
    $('#div-alert-message').html(m);
    $('#modal-alert').modal('show');
  },
  SetAutoTemp: function() {
    if($('#set-auto-temp').prop('checked')) {
      wmGCommands.SetTempOn.GParams = "S"+$('#auto-temp-interval').val();
      wmWebSoket.Send(wmGCommands.SetTempOn);
    } else { wmWebSoket.Send(wmGCommands.SetTempOff); }
  },
  SetFanSpeed: function(rv) {
    rv = parseInt(rv);
    jsLog.Debug("Set fan speed to: "+rv+"%");
    let fsv = wmTools.FormatNumber(wmTools.GetNumPercent(rv,255),0);
    $('#div-fan-speed-current').html(rv+"%<span class='badge badge-success ml-1'>"+fsv+"</span>");
    $('#div-fan-speed-set').html(rv+"%<span class='badge badge-success ml-1'>"+fsv+"</span>");
    if(rv===0) { wmWebSoket.Send(wmGCommands.FanOff); }
    else {
      wmGCommands.FanOn.GParams = "S"+fsv;
      wmWebSoket.Send(wmGCommands.FanOn);
    }
    WmChartsData.FanSpeed.DataUpdate(rv);
  },
  ResetSdFileCounters() {
    $('#list-sd-content').empty();
    $('#div-sdlist-file-count').text(0);
    $('#div-sdlist-folder-count').text(0);
  },
  UpdateTemperatures: function(dt){
    $('#div-temp-extruder-detect').text(dt.ExtruderTemp);
    $('#div-temp-extruder-set').text(dt.ExtruderSet);
    $('#div-temp-extruder-unit').text(wmSettings.TempUnit.LABEL);
    if(dt.BedTemp !== null) {
      $('#div-temp-bed-detect').text(dt.BedTemp);
      $('#div-temp-bed-set').text(dt.BedSet);
      $('#div-temp-bed-unit').text(wmSettings.TempUnit.LABEL);
    }
  },
  SetPrinterStatusInfo: function(s, m) {
    if(!s) {
      $('#div-pstatus-sdprint').removeClass("badge-danger").addClass("badge-info");
      $('#div-pstatus-sdprint').html("Ready to print");
      $('#div-pstatus-timer').collapse('hide');
      $('#div-pstatus-progress').collapse('hide');
    } else {
      $('#div-pstatus-sdprint').removeClass("badge-danger badge-info").addClass("badge-success");
      $('#div-pstatus-sdprint').html("Printing in progress");
      $('#div-pstatus-timer').collapse('show');
      $('#div-pstatus-progress').collapse('show');
    }
  },
  SetSteppers: function(o) {
    if(o.id==="set-stepper-all") {
      WmControls.SetCheckStatus(WmButtonGroups.Stepper,o.checked);
      if(o.checked) { wmWebSoket.Send(wmGCommands.StepEnableAll); } else { wmWebSoket.Send(wmGCommands.StepDisableAll); }
    } else {
      if(o.id==="set-stepper-x") { wmGCommands.StepEnable.GParams = "X"; }
      else if(o.id==="set-stepper-y") { wmGCommands.StepEnable.GParams = "Y"; }
      else if(o.id==="set-stepper-z") { wmGCommands.StepEnable.GParams = "Z"; }
      else if(o.id==="set-stepper-e") { wmGCommands.StepEnable.GParams = "E"; }
      if(o.checked) { wmWebSoket.Send(wmGCommands.StepEnable); } else { wmWebSoket.Send(wmGCommands.StepDisable); }
    }
  },
};

var WmChartsData = {
  Temperatures: {
    TimeFormat: 'HH:mm:ss',
    Speed: 1000,
    Scale: 1,
    Extruder: {
      Label: "Extruder",
      BorderColor: wmColors.RedCoral,
      BgColor: wmColors.RedCoral,
      Data:[]
    },
    Bed: {
      Label: "Bed",
      BorderColor: wmColors.Blue,
      BgColor: wmColors.Blue,
      Data:[]
    },
    AddEmpty: function(arr, n) {
      for(var i = 0; i < n; i++) {
        let xd = moment().subtract((n - i) * WmChartsData.Temperatures.Speed, 'milliseconds').toDate();
        arr.push({ x: xd, y: null });
      }
    },
    DataUpdate(di) {
      let dt = new Date();
      WmChartsData.Temperatures.Extruder.Data.push({ x: dt, y: di.ExtruderTemp });
      WmChartsData.Temperatures.Extruder.Data.shift();
      if(di.BedTemp !== null) {
        WmChartsData.Temperatures.Bed.Data.push({ x: dt, y: di.BedTemp });
        WmChartsData.Temperatures.Bed.Data.shift();
      }
      requestAnimationFrame(WmCharts.Advance);
    }
  },
  FanSpeed: {
    Values: {
      Label: ["Fan speed",""],
      BgColor: [wmColors.GreenSuc, wmColors.Grey],
      Data: [0,100]
    },
    DataUpdate(di) {
      WmChartsData.FanSpeed.Values.Data[0] = di;
      WmChartsData.FanSpeed.Values.Data[1] = 100 - di;
      if(di < 30) { WmChartsData.FanSpeed.Values.BgColor[0] = wmColors.GreenSuc; }
      else if(di < 60) { WmChartsData.FanSpeed.Values.BgColor[0] = wmColors.Yellow; }
      else if(di < 80) { WmChartsData.FanSpeed.Values.BgColor[0] = wmColors.Orange; }
      else { WmChartsData.FanSpeed.Values.BgColor[0] = wmColors.Red; }
      requestAnimationFrame(function() { WmCharts.FanSpeed.CanvasItem.update()});
    }
  }
};

var WmCharts = {
  Temperatures: {
    CanvasItem: null,
    Config: {
      type: 'line',
      data: {
        datasets: [{
          label: WmChartsData.Temperatures.Extruder.Label,
          data: WmChartsData.Temperatures.Extruder.Data,
          backgroundColor: WmChartsData.Temperatures.Extruder.BgColor,
          borderColor: WmChartsData.Temperatures.Extruder.BorderColor,
          borderWidth: 2,
          fill: false,
          pointRadius: 1.5
        },{
          label: WmChartsData.Temperatures.Bed.Label,
          data: WmChartsData.Temperatures.Bed.Data,
          backgroundColor: WmChartsData.Temperatures.Bed.BgColor,
          borderColor: WmChartsData.Temperatures.Bed.BorderColor,
          borderWidth: 2,
          fill: false,
          pointRadius: 1.5
        }]
      },
      options: {
        responsive: true,
        animation: { duration: WmChartsData.Temperatures.Speed * 1.5, easing:'linear' },
        scales: {
          xAxes: [{ type:'time', time:{ displayFormats: { second: 'HH:mm:ss'} }, scaleLabel: { display: false } }],
          yAxes: [{ ticks: { min: 0} }]
        }
      }
    },
    DisplayAxis: function(ck) {
      if(ck.id==="chart-show-extruder") {
        WmCharts.Temperatures.CanvasItem.getDatasetMeta(0).hidden = ck.checked===true ? false : true;
        $('#chart-show-extruder-label').text(ck.checked===true ? "Show" : "Hide");
      }
      else if(ck.id==="chart-show-bed") {
        WmCharts.Temperatures.CanvasItem.getDatasetMeta(1).hidden = ck.checked===true ? false : true;
        $('#chart-show-bed-label').text(ck.checked===true ? "Show" : "Hide");
      }
      if(!$('#set-auto-temp').prop('checked')){ WmCharts.Temperatures.CanvasItem.update(); }
    }
  },
  FanSpeed: {
    CanvasItem: null,
    Config: {
      type: 'doughnut',
      data: {
        datasets: [{
          data: WmChartsData.FanSpeed.Values.Data,
          backgroundColor: WmChartsData.FanSpeed.Values.BgColor,
        }],
        labels: WmChartsData.FanSpeed.Values.Label
      },
      options: {
        responsive: true,
        circumference: Math.PI,
        rotation: -Math.PI,
        legend: { display: false },
        tooltips: { enabled: false },
        title: { display: false },
        animation: {
          animateScale: true,
          animateRotate: true,
          onComplete: function () {
            var ctx = this.chart.ctx;
            ctx.font = "14pt Verdana";
            ctx.textAlign = 'center';
            ctx.textBaseline = 'bottom';
            ctx.fillStyle = wmColors.Black;
            let ds = this.data.datasets;
            let model = ds[0]._meta[Object.keys(ds[0]._meta)[0]].data[0]._model;
            let xp = wmTools.FormatNumber(model.x,0), yp = wmTools.FormatNumber(model.y,0);
            jsLog.Debug("Drawing speed gauge percentage label position (x,y): "+xp+","+yp);
            ctx.fillText(WmChartsData.FanSpeed.Values.Data[0]+'%', xp, yp);
          }
        }
      }
    }
  },
  Init: function() {
    WmChartsData.Temperatures.AddEmpty(WmChartsData.Temperatures.Extruder.Data, 20);
    WmChartsData.Temperatures.AddEmpty(WmChartsData.Temperatures.Bed.Data, 20);
    WmCharts.Temperatures.CanvasItem = new Chart(document.getElementById('chart-temps'), WmCharts.Temperatures.Config);
    WmCharts.FanSpeed.CanvasItem = new Chart(document.getElementById('chart-fanspeed'), WmCharts.FanSpeed.Config);
  },
  Advance: function() {
    if (WmChartsData.Temperatures.Extruder.Data[0] !== null && WmChartsData.Temperatures.Extruder.Scale < 4) { WmCharts.Temperatures.CanvasItem.update(); }
    WmCharts.Temperatures.CanvasItem.update();
  },
  SetTempReport: function(tr) {
    let temps = {
      ExtruderTemp: tr[0].replace(/T:/,""),
      ExtruderSet: tr[1],
      BedTemp: tr[2]==="undefined" ? null : tr[2].replace(/B:/,""),
      BedSet: tr[3]==="undefined" ? null : tr[3]
    };
    WmChartsData.Temperatures.DataUpdate(temps);
    WmControls.UpdateTemperatures(temps);
  }
};

var WmConsole = {
  Clear: function() {
    jsLog.Debug("Clearing console message list...");
    $('#gcommand-console-list').empty();
    wmLogBuffer.length = 0;
    WmConsole.Trace(new wmLogItem("Console list cleared by user", wmEnums.WSMsgDirection.RECEIVED, wmEnums.ConsoleLevels.SUCCESS));
  },
  Export: function() {
    jsLog.Debug("Exporting console message list...");
    var fdt = new Array();
    // DA FINIRE LA PRIMA RIGA NOMI CAMPI
    for(i=0; i<wmLogBuffer.length; i++) { fdt.push(wmLogBuffer[i].ToCsv()+"\n"); }
    wmTools.FileDownload("e4dbox_log.csv", "text/csv;charset=utf-8", fdt);
  },
  SetMessageSymbol: function() {
    if($('#set-log-symbol').val()==="icon") {
      jsLog.Verbose("Set message symbol icon collapse panel to 'show'",this);
      WmConsole.SetSymbolIcon();
      $('#div-log-symbol-icon').collapse('show');
    } else {
      jsLog.Verbose("Set message symbol icon collapse panel to 'hide'",this);
      $('#div-log-symbol-icon').collapse('hide');
    }
  },
  SetSymbolIcon: function() {
    let ctrl = parseInt($('#set-log-symbol-icon').val());
    let hc = {s:null, r:null};
    jsLog.Verbose("Set settings symbol sample fields ("+ctrl+")");
    if(ctrl===0) { hc.s = wmIcons.ChevronUp.ToHtml(); hc.r = wmIcons.ChevronDown.ToHtml(); }
    else if(ctrl===1) { hc.s = wmIcons.LongArrowUp.ToHtml(); hc.r = wmIcons.LongArrowDown.ToHtml(); }
    else if(ctrl===2) { hc.s = wmIcons.ChevronLeft.ToHtml(); hc.r = wmIcons.ChevronRight.ToHtml(); }
    else if(ctrl===3) { hc.s = wmIcons.LongArrowLeft.ToHtml(); hc.r = wmIcons.LongArrowRight.ToHtml(); }
    $('#div-log-symbol-icon-sample-s').html(hc.s);
    $('#div-log-symbol-icon-sample-r').html(hc.r);
  },
  Trace: function(litem) {
    wmLogBuffer.push(litem);
    let clist = $('#gcommand-console-list');
    if (wmSettings.ConsoleDirection === wmEnums.ConsoleDirection.APPEND) { clist.append(litem.ToLoglist()); } else { clist.prepend(litem.ToLoglist()); }
    $('#log-counter-badge').text(wmLogBuffer.length);
  },
  TraceSdFile: function(litem) {
    if(litem.SdFile === "Begin file list") { wmSdListCounter.Reset(); }
    if(litem.SdFile !== "Begin file list" && litem.SdFile !== "End file list") {
      wmSdListCounter.FILES++;
      if(litem.SdFile.indexOf("/")>-1) { wmSdListCounter.FOLDERS++; }
    }
    $('#list-sd-content').append(litem.ToSdFileList());
    $('#div-sdlist-file-count').text(wmSdListCounter.FILES);
    $('#div-sdlist-folder-count').text(wmSdListCounter.FOLDERS);
    if(litem.SdFile === "End file list") { WmControls.Enable(WmButtonGroups.FileManagement); }
  }
};

var WmAutostart = {
  SetDefaultPanel: function() {
    jsLog.Verbose("Set default shown panel ("+wmSettings.DefaultPanel+")");
    if (wmSettings.DefaultPanel == wmEnums.Panels.STATUS) { $('#accordion-panel-status-body').addClass("show"); }
    else if (wmSettings.DefaultPanel === wmEnums.Panels.CONTROLS) { $('#accordion-panel-controls-body').addClass("show"); }
    else if (wmSettings.DefaultPanel === wmEnums.Panels.TEMP) { $('#accordion-panel-temp-body').addClass("show"); }
    else if (wmSettings.DefaultPanel === wmEnums.Panels.FILES) { $('#accordion-panel-file-body').addClass("show"); }
    else if (wmSettings.DefaultPanel === wmEnums.Panels.CONSOLE) { $('#accordion-panel-console-body').addClass("show"); }
  },
  SetShownPanel: function(p) {
    jsLog.Verbose("Set shown panel ("+p+")");
    if (p === wmEnums.Panels.STATUS) { $('#accordion-panel-status-body').removeClass("hide").addClass("show"); } else { $('#accordion-panel-status-body').removeClass("show").addClass("hide"); }
    if (p === wmEnums.Panels.CONTROLS) { $('#accordion-panel-controls-body').removeClass("hide").addClass("show"); } else { $('#accordion-panel-controls-body').removeClass("show").addClass("hide"); }
    if (p === wmEnums.Panels.TEMP) { $('#accordion-panel-temp-body').removeClass("hide").addClass("show"); } else { $('#accordion-panel-temp-body').removeClass("show").addClass("hide"); }
    if (p === wmEnums.Panels.FILES) { $('#accordion-panel-file-body').removeClass("hide").addClass("show"); } else { $('#accordion-panel-file-body').removeClass("show").addClass("hide"); }
    if (p === wmEnums.Panels.CONSOLE) { $('#accordion-panel-console-body').removeClass("hide").addClass("show"); } else { $('#accordion-panel-console-body').removeClass("show").addClass("hide"); }
  },
  SetGCommandChecksum: function() {
    cs = wmGCommandItem.CalcChecksum($('#text-gcommand').val());
    $('#checksum-gcommand-value').text(cs);
    jsLog.Debug("Set GCommand checksum ("+cs+")");
  },
  SetGCommandPresetList: function() {
    jsLog.Verbose("Fill GCommand preset list");
    let lip = document.getElementById("list-presets").getElementsByTagName("li");
    let i=0;
    if(lip.length === 1) {
      Object.keys(wmGCommands).forEach(key => {
        if(wmGCommands[key].GCode != "" && wmGCommands[key].Supported) {
          let gp = wmGCommands[key].GParams !== null ? wmTools.StringFormat(wmGCommands[key].GParams,wmGCommands[key].Values) : '';
          var lib = "<button type=\"button\" class=\"list-group-item list-group-item-action p-1\" onclick=\"WmButtons.GCommandSetPreset('"+wmGCommands[key].GCode+" "+gp+"')\">";
          lib += "<span class=\"h5\"><span class=\"badge badge-success mr-1\" style=\"width:100px;\">"+wmGCommands[key].GCode+" "+gp+"</span></span>";
          lib += wmGCommands[key].Description+"</button>";
          $('#list-presets').append(lib);
          i++;
        }
      });
    }
    jsLog.Verbose("Fill GCommand preset list completed ("+i+" items)");
  },
  SetWmSettingsControls: function() {
    jsLog.Verbose("Set settings controls fields");
    $('#set-default-panel').val(wmSettings.DefaultPanel);
    $('#set-auto-connect').bootstrapToggle(wmSettings.AutoConnect==true ? "on" : "off");
    $('#set-default-autotemp').val(wmSettings.AutoTempInterval);
    $('#set-default-tempunit').val(wmSettings.TempUnit.VALUE);
    $('#set-log-level').val(wmSettings.LogLevel);
    if(wmSettings.ConsoleDirection===wmEnums.ConsoleDirection.APPEND) {
      $('#set-trace-mode-prepend').prop('checked',false);
      $('#set-trace-mode-append').prop('checked',true);
    } else {
      $('#set-trace-mode-append').prop('checked',false);
      $('#set-trace-mode-prepend').prop('checked',true);
    }
    $('#set-log-symbol').val(wmSettings.SymbolMode);
    if(wmSettings.SymbolMode==='letter') { $('#div-log-symbol-icon').collapse('hide'); }
    else { $('#div-log-symbol-icon').collapse('show'); SetConsoleSymbolIcon(); }
  },
  SetAutotempDefault: function() {
    $('#auto-temp-interval').val(wmSettings.AutoTempInterval);
  }
};

$(document).ready(function () {
  $('.accordion-always-open').on('show.bs.collapse', function () { $(this).data('isShowing', true); });
  $('.accordion-always-open').on('hide.bs.collapse', function (event) {
    if (!$(this).data('isShowing')) { event.preventDefault(); }
    $(this).data('isShowing', false);
  });
  jsLog.Debug("Browser in use: "+wmTools.GetBrowser());
  jsLog.Debug("Browser sizes: "+wmTools.GetScreenSize());

  // AutoStar Actions
  WmAutostart.SetWmSettingsControls();
  WmAutostart.SetDefaultPanel();
  WmAutostart.SetGCommandPresetList();
  WmAutostart.SetAutotempDefault();

  // EVENTS: Buttons
  $('#btn-clear-console').click(function() { WmButtons.ConsoleListClear(); });
  $('#btn-export-console').click(function() { WmButtons.ConsoleListExport(); });
  $('#btn-file-proc').click(function(){ WmUpload.FileProcess(); });
  $('#btn-file-proc-cancel').click(function() { WmUpload.Cancel(); });
  $('#btn-file-upload').click(function(){ WmButtons.UploadSdFile(); });
  $('#btn-gcommand').click(function() { WmButtons.SendGcommand(); });
  $('#btn-get-sdcontent').click(function() { WmButtons.GetSdContentList(); });
  $('#btn-move-home-all').click(function() { WmButtons.SetPositionHome(this); });
  $('#btn-move-home-x').click(function() { WmButtons.SetPositionHome(this); });
  $('#btn-move-home-y').click(function() { WmButtons.SetPositionHome(this); });
  $('#btn-move-home-z').click(function() { WmButtons.SetPositionHome(this); });
  $('#btn-save-settings').click(function() { WmButtons.SaveSettings(); });
  $('#btn-sdfile-delete-modal').click(function() { WmButtons.DeleteSdSelected(); });
  $('#btn-sdfile-print-modal').click(function() { WmButtons.PrintSdSelected(); });
  $('#btn-set-sdinit').click(function() { WmButtons.SdInit(); });
  $('#btn-set-sdrelease').click(function() { WmButtons.SdRelease(); });
  $('#btn-wsconnect').click(function() { WmButtons.WsConnect(); });
  $('#btn-wsconnect-modal').click(function() { WmButtons.WsConnect(); });

  // EVENTS: Inputs fields
  $('#text-gcommand').keyup(function()  { WmAutostart.SetGCommandChecksum(); });
  $('#text-gcommand').change(function() { WmAutostart.SetGCommandChecksum(); });
  $('#set-log-symbol').change(function() { WmConsole.SetMessageSymbol(); });
  $('#set-log-symbol-icon').change(function() { WmConsole.SetSymbolIcon(); });
  $('#file-upload').change( function() { WmControls.SetUpload(); });
  $('#set-auto-temp').change( function() { WmControls.SetAutoTemp(); });
  $('#auto-temp-interval').change( function() { WmControls.SetAutoTemp();} );
  $('#chart-show-extruder').change( function() { WmCharts.Temperatures.DisplayAxis(this); });
  $('#chart-show-bed').change( function() { WmCharts.Temperatures.DisplayAxis(this); });
  $('#fan-speed-range').on("change", function() { WmControls.SetFanSpeed(this.value); });
  $('#set-stepper-all').change( function() { WmControls.SetSteppers(this); });
  $('#set-stepper-e').change( function() { WmControls.SetSteppers(this); });
  $('#set-stepper-x').change( function() { WmControls.SetSteppers(this); });
  $('#set-stepper-y').change( function() { WmControls.SetSteppers(this); });
  $('#set-stepper-z').change( function() { WmControls.SetSteppers(this); });

  // Autorun onload
  WmConsole.Trace(new wmLogItem("Ready", wmEnums.WSMsgDirection.SENT, wmEnums.ConsoleLevels.SUCCESS));
  if(wmSettings.AutoConnect===true) { wmWebSoket.Connect(); }
  window.onload = function() {
    WmCharts.Init();
    WmCharts.Advance();
  };

  WmControls.Enable(WmButtonGroups.All());
});
