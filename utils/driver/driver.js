'use strict';

const SOCK_PATH = 'rumour.media';

const abs = require('abstract-socket');
const Later = require('later');
const Path = require('path');

let connected = false;

const filenames = [
  'imgs/slide-1.jpg',
  'imgs/slide-2.jpg',
  'imgs/slide-4.jpg',
];

let currentlyPlaying = 0;

if (filenames.length == 0) {
  console.error('You need to add some filenames in here!');
  process.exit(1);
}

const client = abs.connect('\0' + SOCK_PATH);

const playAds = function () {
  if (currentlyPlaying >= filenames.length) {
    currentlyPlaying = 0;
  }
  const fullPath = Path.join(__dirname, filenames[currentlyPlaying]);
  client.write(fullPath+'\0');
  ++currentlyPlaying;
}

const adSlotSchedule = Later.parse.text(`every 5 seconds`);
Later.setInterval(playAds, adSlotSchedule);
