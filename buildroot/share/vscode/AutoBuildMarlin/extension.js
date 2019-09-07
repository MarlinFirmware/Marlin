'use strict';

var vscode = require('vscode');

function activate(context) {

  console.log('Extension "AutoBuildMarlin" is now active!');

  var NEXT_TERM_ID = 1;
  var pio_build     = vscode.commands.registerCommand('piobuild',     function () {
    vscode.commands.executeCommand('workbench.action.files.saveAll');
    const terminal = vscode.window.createTerminal(`AB Build #${NEXT_TERM_ID++}`);
    terminal.show(true);
    terminal.sendText("python buildroot/share/atom/auto_build.py build");
    });
  var pio_clean     = vscode.commands.registerCommand('pioclean',     function () {
    const terminal = vscode.window.createTerminal(`AB Clean #${NEXT_TERM_ID++}`);
    terminal.show(true);
    terminal.sendText("python buildroot/share/atom/auto_build.py clean");
  });
  var pio_upload    = vscode.commands.registerCommand('pioupload',    function () {
    vscode.commands.executeCommand('workbench.action.files.saveAll');
    const terminal = vscode.window.createTerminal(`AB Upload #${NEXT_TERM_ID++}`);
    terminal.show(true);
    terminal.sendText("python buildroot/share/atom/auto_build.py upload");
  });
  var pio_traceback = vscode.commands.registerCommand('piotraceback', function () {
    vscode.commands.executeCommand('workbench.action.files.saveAll');
    const terminal = vscode.window.createTerminal(`AB Traceback #${NEXT_TERM_ID++}`);
    terminal.show(true);
    terminal.sendText("python buildroot/share/atom/auto_build.py traceback");
  });

  context.subscriptions.push(pio_build);
  context.subscriptions.push(pio_clean);
  context.subscriptions.push(pio_upload);
  context.subscriptions.push(pio_traceback);
}
exports.activate = activate;

// this method is called when your extension is deactivated
function deactivate() {
}
exports.deactivate = deactivate;
