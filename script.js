async function loadData()
{
    let response = await fetch("/weather");

    let data = await response.json();

    document.getElementById("temp").innerHTML=data.temperature+" °C";

    document.getElementById("humidity").innerHTML=data.humidity+" %";

    document.getElementById("wind").innerHTML=data.wind+" km/h";

    document.getElementById("wifi").innerHTML=data.rssi+" dBm";
}

loadData();

setInterval(loadData,30000);