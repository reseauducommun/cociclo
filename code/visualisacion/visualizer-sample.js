/*
 * Copyright 2013 Boris Smus. All Rights Reserved.

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


var WIDTH = 640;
var HEIGHT = 360;

// Interesting parameters to tweak!
var SMOOTHING = 0.8;
//var FFT_SIZE = 2048;
var FFT_SIZE = 128;

function VisualizerSample() {
  this.analyser = context.createAnalyser();

  this.analyser.connect(context.destination);
  this.analyser.minDecibels = -140;
  this.analyser.maxDecibels = 0;
  loadSounds(this, {
    buffer: 'condenser.wav'
  });
    
// Create the filter
var filter = context.createBiquadFilter();
// Create the audio graph.
// source.connect(filter);
// filter.connect(context.destination);
// Create and specify parameters for the low-pass filter.
filter.type = 0; // High-pass filter. See BiquadFilterNode docs
filter.frequency.value = 100; // Set cutoff to 600 HZ
    
    
  this.freqs = new Uint8Array(this.analyser.frequencyBinCount);

  this.isPlaying = false;
  this.startTime = 0;
  this.startOffset = 0;
}

// Toggle playback
VisualizerSample.prototype.togglePlayback = function() {
  if (this.isPlaying) {
    // Stop playback
    this.source[this.source.stop ? 'stop': 'noteOff'](0);
    this.startOffset += context.currentTime - this.startTime;
    console.log('paused at', this.startOffset);
    // Save the position of the play head.
  } else {
    this.startTime = context.currentTime;
    console.log('started at', this.startOffset);
    this.source = context.createBufferSource();
    // Connect graph
    this.source.connect(this.analyser);
    this.source.buffer = this.buffer;
    this.source.loop = true;
    // Start playback, but make sure we stay in bound of the buffer.
    this.source[this.source.start ? 'start' : 'noteOn'](0, this.startOffset % this.buffer.duration);
    // Start visualizer.
    requestAnimFrame(this.draw.bind(this));
  }
  this.isPlaying = !this.isPlaying;
}


VisualizerSample.prototype.draw = function() {
  this.analyser.smoothingTimeConstant = SMOOTHING;
  this.analyser.fftSize = FFT_SIZE;

  // Get the frequency data from the currently playing music
  this.analyser.getByteFrequencyData(this.freqs);
 // this.analyser.getByteTimeDomainData(this.times); // pour le time domain
 // console.log(this.analyser.frequencyBinCount);
    
  var width = Math.floor(1/this.freqs.length, 10);

  var canvas = document.querySelector('canvas');
  var drawContext = canvas.getContext('2d');
  canvas.width = WIDTH;
  canvas.height = HEIGHT;
    
    var sum = 0;
    var moyenne = 0;
    // Faire la moyenne des valeurs dans le tableau des fréquences    
    for (var i = 0; i < this.analyser.frequencyBinCount; i++) {
     // console.log("i = : "+i);  // retourne 0-63
     sum += this.freqs[i];
    }
    
    moyenne = Math.round(sum/this.analyser.frequencyBinCount);
    // Afficher la moyenne des fréquences sous forme de texte
    drawContext.font="30px Verdana";
    drawContext.fillText(moyenne,10,50);
    
  // Draw the frequency domain chart.
  for (var i = 0; i < this.analyser.frequencyBinCount; i++) {
     // console.log("i = : "+i);  // retourne 0-63
     
    var value = this.freqs[i];
    var percent = value / 256;
    var height = HEIGHT * percent;
    var offset = HEIGHT - height - 1;
    var barWidth = WIDTH/this.analyser.frequencyBinCount;
    var hue = i*1.1;
    drawContext.fillStyle = 'hsl(' + hue + ', 100%, 20%)';
    drawContext.fillRect(i * barWidth, offset, barWidth, height);
  }

  if (this.isPlaying) {
    requestAnimFrame(this.draw.bind(this));
  }
}

VisualizerSample.prototype.getFrequencyValue = function(freq) {
  var nyquist = context.sampleRate/2;
  var index = Math.round(freq/nyquist * this.freqs.length);
  return this.freqs[index];
   
}