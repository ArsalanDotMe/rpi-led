'use strict';

const SOCK_PATH = 'rumour.media';

const ipc = require('node-ipc');
const Later = require('later');

let connected = false;

const filenames = [];
let currentlyPlaying = 0;

if (filenames.length == 0) {
  console.error('You need to add some filenames in here!');
  process.exit(1);
}

const playAds = function () {
  if (currentlyPlaying >= filenames.length) {
    currentlyPlaying = 0;
  }
  ipc.of.socktest.emit('message', filenames[currentlyPlaying]);
  ++currentlyPlaying;
}

ipc.connectTo(
  'socktest',
  SOCK_PATH,
  () => {
    // socket now created
    ipc.of.socktest.on('connect', () => {
      console.log('## connected to server ##');
      //ipc.of.socktest.emit('message', 'na na');
      connected = true;
      const adSlotSchedule = Later.parse.text(`every 5 seconds`);
      Later.setInterval(playAds, adSlotSchedule);
    });

    ipc.of.socktest.on('disconnect', () => {
      console.log('## disconnected from server ##');
      connected = false;
    });

    ipc.of.socktest.on('message', (buffer) => {
      const msg = buffer.toString();
      console.log('Server replied: ', msg);
    });
  }
)