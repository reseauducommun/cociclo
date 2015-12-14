///////////////////////////////////////////////////////////////////////////////////////////////////////
//  lecture d'une valeur du ADC du esp8266                                                           //
//  cordova plugin add cordova-plugin-geolocation                                                    //
//  cordova plugin add cordova-plugin-chrome-apps-sockets-tcp                                        //
//  cordova plugin add cordova-plugin-chrome-apps-sockets-udp                                        //
//  cordova plugin add cordova-plugin-file                                                           //
//  cordova plugin add cordova-plugin-nativeaudio                                                    //
//  code arduino : esp8266_server                                                                    //
//  https://github.com/evothings/evothings-examples/blob/master/examples/esp8266/esp8266/esp8266.ino //
//  utilities : coolTerm (OSX), SocketTest3                                                          //
///////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////    //[elongitude],[elatitude],[ealtitude],[eaccuracy],[ealtitudeAccuracy],[eheading],[espeed],[etimestamp]
//      chaque point gps possede 8 donnees correspondantes                                                  
///////////////////////////////////////////////////////////////////////////////////////////////////////

//  ACHTUNG! ajoutez  <preference name="AndroidPersistentFileLocation" value="Compatibility" />
//  en config.xml :-(

elatitude =        0;
elongitude =        0;
ealtitude =        0;           
eaccuracy =       0;           
ealtitudeAccuracy = 0;  
eheading =       0;           
espeed =           0;         
etimestamp =       0; 
d = new Date();
uneDate = d.getDate() + "-" +  d.getMonth() + "-" + d.getFullYear() + "-" + d.getHours() + "-" +  d.getMinutes() + "-" +  d.getSeconds(); 
nomFichier = 'Arles-' + uneDate + '.TXT';
points = [];
//delToggle = true; // declare et initialise une variable globale
CO=42;
donnees = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0];
sonido = 0;

var app = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();
    },
    // Bind Event Listeners. Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        
        document.addEventListener('deviceready', this.onDeviceReady, false);
        
        ////////////// ecoute du gps ////////////////////  
        
        var options = { timeout: 30000, enableHighAccuracy: true, maximumAge: 9000 }; 
        //'maximum age' semble nécessaire pour utiliser le récepteur gps et pas uniquement le wifi
        watchID = navigator.geolocation.watchPosition(onSuccess, onError, options);  
        
        var options = { frequency: 200 };  // Update every 5th of a second
        ///////var watchID = navigator.accelerometer.watchAcceleration(onSuccess, onError, options);
        
        ////////// pour ios, pas de 'onClick' //////////////////////////////////////////
        
        document.getElementById('ouibutton').addEventListener('touchend', connexion, false);
        document.getElementById('nobutton').addEventListener('touchend', deconnecte, false);
        //   document.getElementById('UDP').addEventListener('touchend', UDP, false);
        document.getElementById('horloge').addEventListener('touchend', horloge, false);
        document.getElementById('ecrit').addEventListener('touchend', ecrit, false);
        document.getElementById('toc').addEventListener('touchend', stop, false);
        //  document.getElementById('envoi').addEventListener('touchend', envoi, false);
        

    },
    
    ////////////////////////////////////////////////////////////////////////////////
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicitly call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        app.receivedEvent('deviceready');
         audioLoad(); // va charger les fichiers audio
        },
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        //alert('Received Event: ' + id);

    }
};

///////////////////////////////////////////////////////////////////////////
//                   Ouvrir la connexion avec le esp8266                 //
///////////////////////////////////////////////////////////////////////////
function connexion(){   
    
    var IPa = document.getElementById("IPAddress").value;
    chrome.sockets.udp.create({}, function(createInfo) 
        {
        socketId = createInfo.socketId;
        // Setup event handler and bind socket.
        chrome.sockets.udp.onReceive.addListener(receiveListener); // pour la valeur de retour
        adresseIP = IPa;
        var PORT = 2390;
        chrome.sockets.udp.bind(socketId,'0.0.0.0',0,onConnectedCallback);      
        });
                   
    function onConnectedCallback(result) {
	          
        if (result == 0) {
        // alert('Connecte par UDP a : ' + adresseIP);   
        document.getElementById("debut").style.display = "none";
        // document.getElementById("connecte").style.display = "block";
        document.getElementById("lectureCO").style.display ="block";
        document.getElementById("controle").style.display = "block";
        document.getElementById("ecriture").style.display = "block";
        // document.getElementById("stop").style.display = "block";
        // document.getElementById("envoi").style.display = "block";
        
        } else {
            
            alert('Failed to connect to ' + adresseIP);
            
            }
        }
    }

/////////////////////////////////////////////////////////////////////////
//                         Appels automatiques                         //        
/////////////////////////////////////////////////////////////////////////

function horloge(){
  
    document.getElementById("lectureCO").style.display ="none";
    document.getElementById("arretCO").style.display ="block";
    myVar = setInterval(UDP, 75);  // appelle la fonction UDP() à chaque 75 ms

    }

//////////////////////////////////////////////////////////////////
//                       stop the clock                         //
//////////////////////////////////////////////////////////////////

function stop(){
    
    document.getElementById("lectureCO").style.display ="block";
    document.getElementById("arretCO").style.display ="none";
    clearInterval(myVar); // arrête l'essai d'envoi d'un requête alors que la 'socket' sera fermée
    //  alert('arret');
    
    }


///////////////////////////////////////////////////////////////////
//                    Demande le data au huzzah                  //       
///////////////////////////////////////////////////////////////////

function UDP(){
    // il faudrait faire une vérification que socketid n'existe pas
    sendString = 'valeurSVP';
    // alert('soquette UDP : ' + socketId);
    // alert(adresseIP);
    // chrome.sockets.udp.send(socketId,stringToBuffer(sendString),'192.168.0.104',2390,
    chrome.sockets.udp.send(socketId,stringToBuffer(sendString),adresseIP,2390,
		function(sendInfo) {
			if (sendInfo.resultCode < 0) {
				alert('Failed to send data')
			}
        });
    ///// alert('envoi reussi');
    }


/////////////////////////////////////////////////////////////////
// chrome.sockets.tcp.onReceive.addListener(function callback) //
//           est-ce qu'on reçoit quelque chose?                //
//         affiche la valeur de CO dans le canvas              //
/////////////////////////////////////////////////////////////////

function receiveListener(info) { // il faudra lui passer la valeur de CO
  
    CO = bufferToString(info.data); // est un ArrayBuffer
    // alert('CO : '+CO); 
    // document.getElementById("contaminacion").innerHTML = CO;  
    
    var canvas = document.getElementById("monCanvas");    
    var ctx = canvas.getContext("2d");
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    
    //CO = (175-Math.round(175*Math.random())); // pour générer des valeurs aléatoires
    //document.getElementById("visualisation").innerHTML = CO; 
        
    donnees.unshift(CO); // on ajoute la plus récente valuer de CO au tableau
    donnees.pop(); // on retire le dernier élément du tableau
    //alert(donnees);
    
    for (i=0;i<donnees.length;i++){
    var my_gradient=ctx.createLinearGradient((i*16),0,(16+(i*16)),175);
    my_gradient.addColorStop(0,"rgba(239, 33, 25, 0.49)");
    my_gradient.addColorStop(1,"rgba(230, 174, 48, 0.28)");
    ctx.fillStyle=my_gradient;
        
    //ctx.fillStyle="#e5dbdb";
    ctx.fillRect((i*16), donnees[i], (16+(i*16)),(canvas.height-donnees[i]));
    // ctx.fillRect(0,100,16,CO);  
       
    }
    
    
//////////////////////////////////////////////////////////
//        après 62 lectures on appelle playa!!          //
//////////////////////////////////////////////////////////   

    sonido=sonido+1;
    sonido = sonido%64;
    // alert('sonido : '+sonido);
    
    if(sonido>62){ // joue à la 63e lecture
       // alert('noooon!!!');
        playa();
        }
     
}

///////////////////////////////////////////////////////////
//                AUDIO!!!AUDIO!!!AUDIO!!!               //
///////////////////////////////////////////////////////////

function audioLoad(){
    
if( window.plugins && window.plugins.NativeAudio ) {

    window.plugins.NativeAudio.preloadSimple( 'bergen0', 'audio/bergen0.mp3', function(msg){
    }, function(msg){
        alert( 'error: ' + msg );
    });
    window.plugins.NativeAudio.preloadSimple( 'bergen1', 'audio/bergen1.mp3', function(msg){
    }, function(msg){
        alert( 'error: ' + msg );
    });
    window.plugins.NativeAudio.preloadSimple( 'bergen2', 'audio/bergen2.mp3', function(msg){
    }, function(msg){
        alert( 'error: ' + msg );
    });
  
    }
}

function playa(){
    
    if(CO<100){
       //  alert('bergen0')
        playaBergen0();
        }
        
    else if(CO > 100 && CO < 200){
        // alert('bergen1')
            playaBergen1();
    }
           
    else {   
    //     alert('bergen2')
        playabergen2();
    }
   
}
    
function playaBergen0(){
    window.plugins.NativeAudio.play( 'bergen0' );
    }   

function playaBergen1(){
    window.plugins.NativeAudio.play( 'bergen1' );
    }

function playaBergen2(){
    window.plugins.NativeAudio.play( 'bergen2' );
    }


//////////////////////////////////////////////////////////
//               Conversions de ArrayBuffer             // 
//////////////////////////////////////////////////////////

function stringToBuffer(string){

	var buffer = new ArrayBuffer(string.length)
	var bufferView = new Uint8Array(buffer)
	
	for (var i = 0; i < string.length; ++i) {

		bufferView[i] = string.charCodeAt(i)
	}

	return buffer
}

function bufferToString(buffer) {
    
	return String.fromCharCode.apply(null, new Uint8Array(buffer))
    
}


/////////////////////////////////////////////////////////
//                   geolocalisation                   //  
/////////////////////////////////////////////////////////

    function onSuccess(position) {
        //alert('nous sommes ici');
        elongitude =        position.coords.longitude;
        elatitude =         position.coords.latitude;
        ealtitude =         position.coords.altitude;           
        eaccuracy =         position.coords.accuracy;           
        ealtitudeAccuracy = position.coords.altitudeAccuracy;  
        eheading =          position.coords.heading;           
        espeed =            position.coords.speed;         
        etimestamp =        position.timestamp;  
  
        var element = document.getElementById('geolocation');
        
     ////////////////// afficher les valeurs dans la page /////////////
       /* element.innerHTML = 'Longitude: '          + elongitude             + '<br />' +
                            'Latitude: '           + elatitude              + '<br />' +        
                            //'Altitude: '           + ealtitude              + '<br />' +
                            //'Accuracy: '           + eaccuracy              + '<br />' +
                            //'Altitude Accuracy: '  + ealtitudeAccuracy      + '<br />' +
                            //'Heading: '            + eheading               + '<br />' +
                            //'Speed: '              + espeed                 + '<br />' +
                            'Timestamp: '          + etimestamp             + '<br />';*/

    // écrire dans un tableau
    
    points.push(etimestamp);
    points.push(elongitude);
    points.push(elatitude);
    points.push(eaccuracy);
    points.push(CO);
               
    }

    // onError Callback receives a PositionError object
    //
    function onError(error) {
        alert('code: '    + error.code    + '\n' +
              'message: ' + error.message + '\n');
    }


///////////////////////////////////////////////////////////
//                       Déconnexion                     //
///////////////////////////////////////////////////////////

function deconnecte(){
    
    clearInterval(myVar); // arrête l'essai d'envoi d'un requête alors que la 'socket' sera fermée

	chrome.sockets.udp.close(socketId, function() {
        // alert('TCP Socket close finished.')
        document.getElementById("debut").style.display = "block";
        document.getElementById("connecte").style.display = "none";
        document.getElementById("controle").style.display = "none";
        document.getElementById("stop").style.display = "none";
	})
}

///////////////////////////////////////////////////////////
//                écriture de fichiers                   //
///////////////////////////////////////////////////////////

function ecrit() {
    
   window.requestFileSystem(LocalFileSystem.PERSISTENT, 5*1024*1024 /*5MB*/, onInitFs, errorHandler);
   //alert('okFile');
    
}
 
function onInitFs(fs) {
    
        // alert('valeurs : '+points[i]);    
        var sortie = points.toString();
        fs.root.getFile(nomFichier, {create: true, exclusive: false}, function(fileEntry) {
              
        //// Create a FileWriter object for our FileEntry (log.txt).       
        fileEntry.createWriter(function(fileWriter) {       
        fileWriter.seek(fileWriter.length); // Start write position at EOF.
        fileWriter.write(sortie);
        alert('donnees sauvegardees');
             
        }, errorHandler); 
         
    }, errorHandler);
     
}

//////////////////////////////////////////////////////////
//                   envoi au serveur                   //
//////////////////////////////////////////////////////////

function envoi(){   
    
alert('envoi au serveur');
    
}

//////////// handling errors from writing ///////////////

function errorHandler(e) {
  var msg = '';

  switch (e.code) {
    case FileError.QUOTA_EXCEEDED_ERR:
      msg = 'QUOTA_EXCEEDED_ERR';
      break;
    case FileError.NOT_FOUND_ERR:
      msg = 'NOT_FOUND_ERR';
      break;
    case FileError.SECURITY_ERR:
      msg = 'SECURITY_ERR';
      break;
    case FileError.INVALID_MODIFICATION_ERR:
      msg = 'INVALID_MODIFICATION_ERR';
      break;
    case FileError.INVALID_STATE_ERR:
      msg = 'INVALID_STATE_ERR';
      break;
    default:
      msg = 'Unknown Error';
      break;
  };

  alert('Error: ' + msg);
}

app.initialize();