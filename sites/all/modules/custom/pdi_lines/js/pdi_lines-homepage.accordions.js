(function ($) {
  Drupal.behaviors.pdiHomeAccordions = {
    attach: function (context, settings) {
      $('.accordions', context).once('HomeAccordions', function () {
        $('.accordion h2').click(accordions);
      });
    }
  };
  function accordions (event) {
    event.preventDefault();
    _this = $(this);
    parent = _this.parent();
    level = parent.attr('data-level');
    console.log(level);
    if (parent.hasClass('acive')) {
      parent.removeClass('active');
      parent.next('h2').removeClass('visible');
    }else{
      parent.addClass('active');
      parent.next('h2').addClass('visible');
    };
  }
})(jQuery);