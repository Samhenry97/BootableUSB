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
});