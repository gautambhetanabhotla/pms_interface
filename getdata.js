let numResults = 10;
let numFields = 1;
var APIKEY = null;
var channelID = null;
let TSrequest = "https://api.thingspeak.com/channels/" + channelID + "/feeds.json?api_key=" + APIKEY + "&results=" + numResults;

fetch("private/Thingspeak.json").then(response => response.json()).then(data => {
    console.log(data);
    channelID = data["channelID"];
    APIKEY = data["readAPIkey"];
});