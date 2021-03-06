$(document).ready(function() {

	$('a[href="#"]').click(function() {
		$('html, body').animate({ scrollTop: 0 }, 'slow');
	});

	$('a[href^="#"]').on('click', function(e) {
	    e.preventDefault();

	    var mobile = $('#mobile').css('display') == 'none';
	    var target = this.hash;
	    var $target = $(target);

	    $('html, body').animate({
	        'scrollTop': $target.offset().top - (mobile ? 50 : 0)
	    }, 'slow', 'swing', function() {
	        window.location.hash = target;
	    });
	});

	$('.navTrigger').click(function() {
		$(this).toggleClass('active');
		$('.navContent').toggleClass('show');

		if($(this).hasClass('active')) {
			$('html, body').css({ overflow: 'hidden', height: '100%' });
			$('#progressContainer').css({ opacity: '0', visibility: 'hidden' });
		} else {
			$('html, body').css({ overflow: 'auto', height: 'auto' });
			$('#progressContainer').css({ opacity: '1', visibility: 'visible' });
		}
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

	var cookie = getCookie('check' + window.location.pathname);
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
	if($change == 100) { $('#progressContainer').addClass('finished'); }

	for(var i = 0; i < checkboxes.length; i++) {
		checkboxes[i].addEventListener('change', function(e) {
			var mobile = $('#mobile').css('display') == 'none';
			var caller = e.srcElement || e.target;
			$change = ($('input:checkbox:checked').length / checkboxes.length) * 100;
			$('#percent').css('width', $change + '%');
			$('#completeamt').html($change.toFixed());
			$(caller).next().html(caller.checked ? 'Mark Incomplete' : 'Mark Complete');

			if($change == 100) {
				$('#progressContainer').addClass('finished');
			} else {
				$('#progressContainer').removeClass('finished');
			
				if(caller.checked) {
					$('html, body').animate({
						'scrollTop': $(caller).parent().parent().parent().next().offset().top - (mobile ? 50 : 0)
					}, 'slow', 'swing');
				}
			}
		});
	}

	$('#clear').click(function() {
		for(var i = 0; i < checkboxes.length; i++) {
			checkboxes[i].checked = false;
			$('#percent').css('width', '0%');
			$('#completeamt').html('0');
			$('#progressContainer').removeClass('finished');
		}
	});

});

window.onbeforeunload = function() {
	var tmp = '';
	var checkboxes = document.querySelectorAll('input[type=checkbox]');
	for(var i = 0; i < checkboxes.length; i++) {
		tmp += checkboxes[i].checked ? '1' : '0';
	}
	setCookie('check' + window.location.pathname, tmp, 365);
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