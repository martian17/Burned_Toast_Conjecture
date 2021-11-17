let fs = require("fs").promises;


let main = async function(){
    let d1 = await fs.readFile("1000.trusted.txt");
    d1 = (
        "0 0\n1 2\n"+
        (await fs.readFile("1000.trusted.txt")).toString()+"\n"+
        (await fs.readFile("2000.trusted.txt")).toString()
    ).split("\n").map(l=>l.trim()).filter(l=>l!=="");
    console.log(d1);
    
    let d2 = await fs.readFile("untrusted.txt");
    d2 = (d2.toString()).split("\n").map(l=>l.trim()).filter(l=>l!=="");
    console.log(d2);
    for(let i = 0; i < d1.length; i++){
        if(d1[i] !== d2[i]){
            console.log("wtf",d1[i],d2[i]);
        }
    }
    d2.filter(s=>{
        s = s.split(" ");
        let i = parseInt(s[0]);
        let n = parseInt(s[1]);
        if(3300 < i && i < 3500 && 1200000 < n && n < 1700000){
            console.log(s);
        }
        return false;
    });
};

main();