//Express
const express = require('express');
const app = express();

//Serve static files
app.use(express.static('public', {
    setHeaders: (res, path, stat) => {
        if (path.endsWith('.wasm')) {
            res.set('Content-Type', 'application/wasm');
        }
    }})
);

//Start server
app.listen(2222, () => {
    console.log('Server started on port 2222');
}
);

