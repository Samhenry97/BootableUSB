$(document).ready(function() {

	$('a[href="#"]').click(function() {
		$('html, body').animate({ scrollTop: 0 }, 'slow');
	});

	$('a[href^="#"]').on('click', function(e) {
	    e.preventDefault();

	    var target = this.hash;
	    var $target = $(target);

	    $('html, body').animate({
	        'scrollTop': $target.offset().top
	    }, 'slow', 'swing', function() {
	        window.location.hash = target;
	    });
	});

	if(navigator.appVersion.indexOf("Win") != -1) {
		$('#windows').toggleClass('suggested');
		$('#detected').html('We\'ve detected that you are running Windows. <br><a class=\'button\' href=\'windows.html\'>Go!</a>');
	} else if(navigator.appVersion.indexOf("Mac") != -1) {
		$('#mac').toggleClass('suggested');
		$('#detected').html('We\'ve detected that you are running Mac OS X. <br><a class=\'button\' href=\'mac.html\'>Go!</a>');
	} else if(navigator.appVersion.indexOf("Linux") != -1) {
		$('#linux').toggleClass('suggested');
		$('#detected').html('We\'ve detected that you are running Linux. <br><a class=\'button\' href=\'linux.html\'>Go!</a>');
	}

	var checkboxes = document.querySelectorAll('input[type=checkbox]');

	var cookie = getCookie('check');
	if(cookie.length == checkboxes.length) {
		for(var i = 0; i < cookie.length; i++) {
			if(cookie[i] == '1') {
				checkboxes[i].checked = true;
			}
		}
	}

	$change = ($('input:checkbox:checked').length / checkboxes.length) * 100;
	$('#percent').css('width', $change + '%');
	$('#completeamt').html($change.toFixed());
	if($change == 100) {
		$('#progressContainer').addClass('finished');
	} else {
		$('#progressContainer').removeClass('finished');
	}

	for(var i = 0; i < checkboxes.length; i++) {
		checkboxes[i].addEventListener('change', function(e) {
			$change = ($('input:checkbox:checked').length / checkboxes.length) * 100;
			$('#percent').css('width', $change + '%');
			$('#completeamt').html($change.toFixed());

			if($change == 100) {
				$('#progressContainer').addClass('finished');
			} else {
				$('#progressContainer').removeClass('finished');
				
				if(e.srcElement.checked) {
					$('html, body').animate({
						'scrollTop': $(e.srcElement).parent().parent().parent().next().offset().top
					}, 'slow', 'swing');
				}
			}
		});
	}

});

window.onbeforeunload = function() {
	var tmp = '';
	var checkboxes = document.querySelectorAll('input[type=checkbox]');
	for(var i = 0; i < checkboxes.length; i++) {
		tmp += checkboxes[i].checked ? '1' : '0';
	}
	setCookie('check', tmp, 365);
}

function setCookie(cname, cvalue, exdays) {
    var d = new Date();
    d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
    var expires = "expires="+d.toUTCString();
    document.cookie = cname + "=" + cvalue + ";" + expires + ";path=/";
}

function getCookie(cname) {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for(var i = 0; i < ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}