let isRational = function(arr){
    for(let i = 0; i < arr.length; i++){
        if(!arr[i][1] || arr[i][0] !== i){
            return false;
        }
    }
    return true;
}

let toast = function(x){
    let arr = new Array(x).fill(0).map((a,i)=>[i,true]);
    let cnt = 0;
    let n = 1;
    do {
        cnt++;
        for(let i = 0; i < n/2; i++){
            let temp = arr[i];
            arr[i] = arr[n-i-1];
            arr[n-i-1] = temp;
        }
        for(let i = 0; i < n; i++){
            arr[i][1] = !arr[i][1];
        }
        n = (n+1)%x;
    } while (!isRational(arr));
    return cnt;
};

for(let i = 2; i < 1000; i++){
    console.log(i,toast(i));
}
