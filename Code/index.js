const express = require("express");
const path = require("path");
var fs = require('fs');
const nodemailer = require('nodemailer');
const child_process = require("child_process");
const app = express();
var mode = "index.html";
app.use(express.static('./public'));

app.get("/test", (req, res)=>{
	test().then((successMessage) => {
		console.log(successMessage);
		res.send(successMessage);
	});
})

app.get("/index", (req,res)=>{
	var response = {
		"LED" : req.query.LED,

        "Switch" : req.query.radio,

        "Num" : req.query.Num,

		"Mode" : req.query.Mode,

		"Sync" : req.query.sync
	}
	if(req.query.Mode == "yes"){
		if (mode != "Detect.html"){
			transporter.sendMail({
				from: 'kerueilin999@gmail.com',
				to: 'kerueilin9@gmail.com',
				subject: '輸入信件主旨',
				html: '輸入信件內容',
			}).then(info => {
				console.log({ info });
			}).catch(console.error);
			mode = "Detect.html";
		}
	}
	else{
		if (mode != "index.html"){
			transporter.sendMail({
				from: 'kerueilin999@gmail.com',
				to: 'kerueilin9@gmail.com',
				subject: '輸入信件主旨',
				html: '輸入信件內容',
			}).then(info => {
				console.log({ info });
			}).catch(console.error);
			mode = "index.html";
		}
	}

	if(req.query.Num != null)
	{
		if (req.query.sync == "Mutex"){
			Mode_Shine_Mutex(req.query.Num);
		} else {
			Mode_Shine_Semaphore(req.query.Num);
		}
	}
	else if(req.query.radio != null)
	{
		if(req.query.LED[0] != "L")
		{
			for(var i = 0; i < req.query.LED.length; i++)
				controlLED(req.query.LED[i], req.query.radio);
		}
		else
		{
			controlLED(req.query.LED, req.query.radio);
		}	
		
	}
	res.sendFile(path.join(__dirname, `/public/${mode}`));
})

function controlLED(LED, POWER)
{
	let process = child_process.execFile('sudo',[
		"./Lab6-2/Lab6_2", LED, POWER
	]);
	process.stdout.on('data', (data) => {
		console.log(`stdout ${data}`);
	})
}

function Mode_Shine_Mutex(TIMES)
{
	let process = child_process.execFile('sudo',[
		"./Lab6-2/Lab6_2", "mutex", TIMES
	]);
	process.stdout.on('data', (data) => {
		console.log(`stdout ${data}`);
	})
}

function Mode_Shine_Semaphore(TIMES)
{
	let process = child_process.execFile('sudo',[
		"./Lab6-2/Lab6_2", "semaphore", TIMES
	]);
	process.stdout.on('data', (data) => {
		console.log(`stdout ${data}`);
	})
}

function test(){
    return new Promise(function(resolve, reject){
        let process = child_process.spawn('python', [ "./python/test.py" ]);

        process.stdout.on('data', (data) => {
            console.log(`stdout: `);
		console.log(data.toString())
            resolve(data.toString())
        });

        process.stderr.on('data', (data) => {
            console.error(`stderr: ${data}`);
            reject(data)
        });
    })
}

const PORT = process.env.PORT || 8080;
app.listen(PORT,()=>{
	
	let process = child_process.execFile('sudo',[
		"./start.sh"
	]);

	console.log(`Server is running on port ${PORT}.`);
});

const transporter = nodemailer.createTransport({
  host: 'smtp.gmail.com',
  port: 465,
  auth: {
    user: 'kerueilin999@gmail.com',
    pass: 'nyjlkvcqbxlscucv',
  },
});

