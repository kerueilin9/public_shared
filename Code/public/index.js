const $LED_1 = document.getElementById("LED1");
const $LED_2 = document.getElementById("LED2");
const $num = document.getElementById("num");

const $submit=$("#Submit");
const $submit2=$("#Submit2");

const led11 = localStorage.getItem('led11');
const led22 = localStorage.getItem('led22');
const result = localStorage.getItem('result');

document. getElementById ( "led1" ). innerHTML = led11 ;
document. getElementById ( "led2" ). innerHTML = led22 ;
document. getElementById ( "result" ).innerHTML = result;

$submit.on('click',async(event)=>{
    var radio = document.querySelector('input[name="radio"]:checked').value;
    if(radio == "on"){
        if ($LED_1.checked) {
            localStorage.setItem('led11', 'LED1：ON');
        }
        if ($LED_2.checked) {
            localStorage.setItem('led22', 'LED2：ON');
        }
    }
    if(radio == "off"){
        if ($LED_1.checked) {
            localStorage.setItem('led11', 'LED1：OFF');
        }
        if ($LED_2.checked) {
            localStorage.setItem('led22', 'LED2：OFF');
        }
    }
})

$submit2.on('click',async(event)=>{
    localStorage.setItem('result', `times：${$num.value}`);
})

