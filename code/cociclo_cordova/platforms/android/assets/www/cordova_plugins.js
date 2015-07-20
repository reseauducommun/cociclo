cordova.define('cordova/plugin_list', function(require, exports, module) {
module.exports = [
    {
        "file": "plugins/cordova-plugin-whitelist/whitelist.js",
        "id": "cordova-plugin-whitelist.whitelist",
        "runs": true
    },
    {
        "file": "plugins/com.cellules.cordova.audiofrequency/www/audiofrequency.js",
        "id": "com.cellules.cordova.audiofrequency.AudioFrequency",
        "clobbers": [
            "cordova.plugins.audiofrequency"
        ]
    }
];
module.exports.metadata = 
// TOP OF METADATA
{
    "cordova-plugin-whitelist": "1.0.0",
    "com.cellules.cordova.audiofrequency": "0.0.1",
    "org.apache.cordova.geolocation": "0.3.12"
}
// BOTTOM OF METADATA
});