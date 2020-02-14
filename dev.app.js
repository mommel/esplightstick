const express = require('express');
const app = express();
const port = process.env.port || 3001;
const imagejson = { data: [
      "demo1.bmp",
      "demo2.bmp",
      "demo3.bmp"
]}
app.use(express.static('dev'));
app.get('/images', (req, res) => {
  res.send(imagejson);
});
app.use('/webfonts', express.static('dev'));
app.use('/images/', express.static('dev/images',
  {extensions: [ 'bmp' ]}
  ));
app.listen(port, function () {
    console.log('Dev Server running on port: ' + port);
});
