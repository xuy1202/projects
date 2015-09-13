
function getNextBatch(){
    window.location.href=$("#nextpage").attr("value");
}

function showContent(){
    var currid = $("#curr_idx").attr("value");
    $("#"+currid+"_sentence").show();
    $("#"+currid+"_meaning").show();
}

function hideContent(){
    var currid = $("#curr_idx").attr("value");
    $("#"+currid+"_sentence").hide();
    $("#"+currid+"_meaning").hide();
}

function doSubstitute(){
    var currid = $("#curr_idx").attr("value");
    $("#"+currid).find(".need_substitute").each(function(){
                $(this).html(('*').repeat($(this).attr("value").length));
                });
}

function reSubstitute(){
    var currid = $("#curr_idx").attr("value");
    $("#"+currid).find(".need_substitute").each(function(){
                $(this).html($(this).attr("value"));
                });
}


function hideall(){
    $(".word_item").each(function(){
                $(this).hide();
                });
}


function showcurr(){
    hideall();
    var currid = $("#curr_idx").attr("value");
    if(currid.length == 0){
        currid = $(".word_item:first").attr("id");
        $("#curr_idx").attr("value", currid);
    }
    $("#"+currid).show();
}
function showprev(){
    var delold = arguments[0] ? arguments[0] : false;
    hideall();
    var currid = $("#curr_idx").attr("value");
    var previd = $("#"+currid).prev().attr("id");
    if(delold){
        $("#"+currid).remove();
    }
    if(typeof(previd) == "undefined"){ 
        previd = $(".word_item:last").attr("id");
        if(typeof(previd) == "undefined"){ 
            getNextBatch();
        }
    }
    $("#curr_idx").attr("value", previd);
    $("#"+previd).show();
}
function shownext(){
    var delold = arguments[0] ? arguments[0] : false;
    hideall();
    var currid = $("#curr_idx").attr("value");
    var nextid = $("#"+currid).next().attr("id");
    if(delold){
        $("#"+currid).remove();
    }
    if(typeof(nextid) == "undefined"){ 
        nextid = $(".word_item:first").attr("id");
        if(typeof(nextid) == "undefined"){ 
            getNextBatch();
        }
    }
    $("#curr_idx").attr("value", nextid);
    $("#"+nextid).show();
}


function spellingShow(){ $("#spelling").show(); $("#spelling_input").val(""); $("#spelling_input").focus(); }
function spellingHide(){ $("#spelling").hide();                                                             }
function disableInput(){ $("#spelling_input").attr("disabled", true); }
function enableInput(){ $("#spelling_input").removeAttr("disabled"); }

function spellingStat(stat){
    $(".spelling_title").each(function(){
                $(this).hide();
                });
    if(stat == "info"){
        $("#spelling").removeClass("alert-success");
        $("#spelling").removeClass("alert-danger");
        $("#spelling").addClass("alert-info"); 
        $("#spelling_tiele_info").show(); 
    }
    if(stat == "succ"){
        $("#spelling").removeClass("alert-info");
        $("#spelling").removeClass("alert-danger");
        $("#spelling").addClass("alert-success"); 
        $("#spelling_tiele_succ").show(); 
    }
    if(stat == "fail"){ 
        $("#spelling").removeClass("alert-info");
        $("#spelling").removeClass("alert-success");
        $("#spelling").addClass("alert-danger"); 
        $("#spelling_tiele_fail").show();
    }
}


function statsChange(){
    var funcstat = $("#funcstat").attr("value");
    if(funcstat == "beg"){ $("#funcstat").attr("value", "end"); }
    if(funcstat == "end"){ $("#funcstat").attr("value", "beg"); }
}

function globalGuide(keysargs){
    var functype = $("#functype").attr("value");
    var funcstat = $("#funcstat").attr("value");
    if(functype=="remember"){
        if(keysargs=="next"){ shownext(); hideContent(); return; }
        if(keysargs=="prev"){ showprev(); hideContent(); return; }
        if(funcstat=="beg" ){ showContent();             return; }
        if(funcstat=="end"){
            if(keysargs=="know"){
                shownext(true);
            }else{
                shownext(false);
            }
            hideContent();
        }
    }
    if(functype=="spelling"){
        if(keysargs=="next"){ shownext(); doSubstitute(); return; }
        if(keysargs=="prev"){ showprev(); doSubstitute(); return; }
        if(funcstat=="beg" ){ 
            reSubstitute(); 
            disableInput(); 
            var spelled = $("#spelling_input").val();
            var currval = $("#" + $("#curr_idx").attr("value") + "_word").attr("value");
            if(spelled == currval){
                spellingStat("succ");
            }else{
                spellingStat("fail");
            }
            return; 
        }
        if(funcstat=="end" ){
            var spelled = $("#spelling_input").val();
            var currid = $("#curr_idx").attr("value");
            if(spelled == $("#"+currid+"_word").attr("value")){
                shownext(true);
            }else{
                shownext(false);
            }
            doSubstitute();
            enableInput();
            spellingShow();
            spellingStat("info");
        }
    }
    return;
}


function keyDown(e) {  
    var keycode = e.which;  
    var realkey = String.fromCharCode(e.which);  
    // 13 for return
    // 32 for space
    // 37 for left/unknow
    // 39 for right/know
    // 38 for up
    // 40 for down
    //alert("按键码: " + keycode + " 字符: " + realkey);  
    if(keycode == 38){ globalGuide("prev"); return false; }
    if(keycode == 40){ globalGuide("next"); return false; }
    if(keycode == 37){ globalGuide("unknow"); statsChange(); return false; }
    if(keycode == 39){ globalGuide("know");   statsChange(); return false; }
    if(keycode==13 || keycode == 32){
        globalGuide("spell"); statsChange(); return false;
    }
}


$(function(){
    showcurr();
    var functype = $("#functype").attr("value");
    if(functype=="remember"){
        hideContent();
        spellingHide();
    }
    if(functype=="spelling"){
        doSubstitute();
        spellingShow();
        spellingStat("info");
    }
    document.onkeydown = keyDown;
});




