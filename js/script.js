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
	    }, 'slow', 'swing', function () {
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

});