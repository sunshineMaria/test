$(function() {
    //数量
    var N = 20;
    //图片地址
    var SNOW_IMG = "http://www.css119.com/demo/img/snow15x15.png";
    var Y = new Array(); 
    var X = new Array();
    var S = new Array();
    var A = new Array();
    var B = new Array();
    var M = new Array();
    var opa = 1;
    var iH = (document.layers) ? window.innerHeight: window.document.body.clientHeight;
    var iW = (document.layers) ? window.innerWidth: window.document.body.clientWidth;
    var allTime = 0;
    iH = iH - 50;
    iW = iW - 100;
    for (i = 0; i < N; i++) {
        Y[i] = Math.round(Math.random() * iH);     
        X[i] = Math.round(Math.random() * iW);     //雪花刚开始出现的位置
        S[i] = Math.round(Math.random());  
        A[i] = Math.round(Math.random()*360);
        B[i] = Math.random() * 0.1 + 0.1;
        M[i] = Math.round(Math.random() * 3 + 3);
    }
    var createSnow = function(N) {
        var $tempConatiner = $("<div>");
        for (i = 0; i < N; i++) {
            var clip = Math.round(Math.random() * 8 + 22);
            var $snow = $("<div>");
            opa = Math.random() * 0.5 + 0.6;
            $snow.attr({
                name: "sn",
                // clip: "0,0," + clip + "," + clip
            }).css({
                left: X[i],
                top: 0,
                // "background-color": "#FFFFF0",
                "z-index": 900,
                "position": "absolute",
                // width: 15 + "px",
                // height: 15 + "px",
                "font-size": clip,
                "color": "#fff",
                opacity: opa, 
                // background: "white",
                // background: "url('" + SNOW_IMG + "') no-repeat"
            }).html("❄").appendTo($tempConatiner);
        }
        $(document.body).append($tempConatiner.html());
        N++;
    }

    function snow() {
        allTime += 40;
        if(allTime < 30000){
        var H = (document.layers) ? window.innerHeight: window.document.body.clientHeight;
        var W = (document.layers) ? window.innerWidth: window.document.body.clientWidth;

        var T = (document.layers) ? window.pageYOffset: document.body.scrollTop;
        var L = (document.layers) ? window.pageXOffset: document.body.scrollLeft;
        H = H - 50;
        W = W - 50;
        $("[name='sn']").each(function(i) {

            var sy = M[i];
            var sx = S[i] * Math.cos(A[i]);
            // var opacity = 0.001;
            // opa -= opacity;
            Y[i] += sy;
            X[i] += sx;
            if (Y[i] > H) {
                Y[i] = -10;
                X[i] = Math.round(Math.random() * W);
                M[i] = Math.round(Math.random() * 3 + 3);
                S[i] = Math.random();
                A[i] = Math.round(Math.random()*360);
                B[i] = Math.random() * 0.1 + 0.1;
                opa = 1;
            }
            $(this).css({
                top: Y[i] + T,
                left: X[i],
                // opacity: o,
            });
            A[i] += B[i];
        });

        setTimeout(function() {
            snow();
        },
        40);
        }
        else {
            $("div").fadeOut('slow');
        }
    }
    createSnow(N);
    snow();
});