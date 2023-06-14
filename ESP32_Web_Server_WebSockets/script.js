var gateway = `ws://${window.location.hostname}/ws`;

var websocket;

window.addEventListener('load', onload);

 

// khởi tạo WebSocket với máy chủ

function onload(event) {

    initWebSocket();

}

 

function getValues(){

    websocket.send("getValues");

}

//Khởi tạo kết nối websocket trên cổng được tạo trước đó

function initWebSocket() {

    console.log('Trying to open a WebSocket connection…');

    websocket = new WebSocket(gateway);

    websocket.onopen = onOpen;

    websocket.onclose = onClose;

    websocket.onmessage = onMessage;

}

 

function onOpen(event) {

    console.log('Connection opened');

    getValues();

}

 

function onClose(event) {

    console.log('Connection closed');

    setTimeout(initWebSocket, 2000);

}

//hàm này sẽ chạy khi di chuyển thanh Slider

function updateSliderPWM(element) {

    var sliderNumber = element.id.charAt(element.id.length-1);

    var sliderValue = document.getElementById(element.id).value;

    document.getElementById("sliderValue"+sliderNumber).innerHTML = sliderValue;

    console.log(sliderValue);

//lấy giá trị từ thành trượt và cập nhật giá trị String tương ứng

    websocket.send(sliderNumber+"s"+sliderValue.toString());

}

 

function onMessage(event) {

    console.log(event.data);

    var myObj = JSON.parse(event.data);

    var keys = Object.keys(myObj);

 

    for (var i = 0; i < keys.length; i++){

        var key = keys[i];

        document.getElementById(key).innerHTML = myObj[key];

        document.getElementById("slider"+ (i+1).toString()).value = myObj[key];

    }

}