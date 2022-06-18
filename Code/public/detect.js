const $title = document.getElementById("temp1");
const $response = document.getElementById("response");
const $submit = $("#Submit");

const active = localStorage.getItem('active');

setInterval(GetResult, 1000);
function GetResult()
{
    if (active == "ON"){
        document.getElementById("temp1").style.fontSize = "36px";
        document.getElementById("temp1").textContent = "感測器偵測中";
        document.getElementById("response").style.fontSize = "40px";
        $.ajax({
            type:"get",
            url:"/test",
            success:function (result)
            {
                console.log(result);
                $response.textContent=`adc_value = ${result}`;
                //$('#response').text(result);
            },
    
            error: function()
            {
                alert("error");
            }
        })
    }
    
}


$submit.on('click',async(event)=>{
    var temp = document.querySelector('input[name="Mode"]:checked').value;
    if(temp == "yes"){
        localStorage.setItem('active', 'ON');
        console.log("123");
    } 
    //else {
    //     localStorage.setItem('active', 'OFF');
    // }
})