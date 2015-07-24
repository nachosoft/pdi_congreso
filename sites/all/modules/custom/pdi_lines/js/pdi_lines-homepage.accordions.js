(function ($) {
  Drupal.behaviors.pdiHomeAccordions = {
    attach: function (context, settings) {
      $('.accordions', context).once('HomeAccordions', function () {
        $('.accordion h2').click(accordions);
      });
    }
  };
  function accordions (event) {
    if ($(this).parent().hasClass('linea_accion')) {
      return;
    };
    event.preventDefault();
    _this = $(this);
    parent = _this.parent();
    level = parent.attr('data-level');
    level_ul = parseInt(level)+1;

    if (parent.hasClass('active')) {
      parent.removeClass('active');
      parent.find('.l-'+level_ul).hide();
    }else{
      parent.addClass('active');
      parent.find('.l-'+level_ul).show();
    };
  }
})(jQuery);