const http = require('node:http');
const fs = require('node:fs');
const path = require('node:path');

const port = Number.parseInt(process.env.SIGNALING_PORT ?? '8080', 10);
const iceConfigPath = path.join(__dirname, 'ice-config.json');
const iceConfig = JSON.parse(fs.readFileSync(iceConfigPath, 'utf-8'));

const server = http.createServer((request, response) => {
  if (request.method === 'GET' && request.url === '/health') {
    response.writeHead(200, { 'content-type': 'application/json' });
    response.end(JSON.stringify({ component: 'signaling_server', status: 'ok', port }));
    return;
  }

  if (request.method === 'GET' && request.url === '/ice') {
    response.writeHead(200, { 'content-type': 'application/json' });
    response.end(JSON.stringify(iceConfig));
    return;
  }

  if (request.method === 'POST' && request.url === '/session') {
    let body = '';
    request.on('data', (chunk) => {
      body += chunk;
    });
    request.on('end', () => {
      const payload = body.length > 0 ? JSON.parse(body) : {};
      response.writeHead(200, { 'content-type': 'application/json' });
      response.end(
        JSON.stringify({
          component: 'signaling_server',
          sessionId: payload.sessionId ?? 'session-alpha',
          accepted: true,
          iceServers: iceConfig.iceServers,
        })
      );
    });
    return;
  }

  response.writeHead(404, { 'content-type': 'application/json' });
  response.end(JSON.stringify({ error: 'not_found' }));
});

server.listen(port, () => {
  process.stdout.write(JSON.stringify({ component: 'signaling_server', status: 'listening', port }) + '\n');
});
