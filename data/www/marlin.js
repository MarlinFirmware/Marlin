document.addEventListener('DOMContentLoaded', () => {
  const ws = new WebSocket(`ws://${location.host}/ws`);

  ws.onmessage = (e) => {
    if (typeof e.data === 'string') {
      let node = document.createElement('li');
      let text = document.createTextNode(e.data);
      node.appendChild(text);
      document.getElementById('serial-output').appendChild(node);
    }
  };

  document.getElementById('serial-command-form').addEventListener('submit', (e) => {
    e.preventDefault();

    let value = document.getElementById('serial-command').value.trim();

    if (!value) return;

    ws.send(`${value}\n`);

    document.getElementById('serial-command').value = '';
  });
});
