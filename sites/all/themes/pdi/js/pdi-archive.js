(function ($) {
  Drupal.behaviors.pdiArchive = {
    attach: function (context, settings) {
      $('#archive', context).once('Archive', function () {
        // add container class
        $('#pdi_admin_menu li').each(function(index, el) {
          $(this).has('ul').addClass('container');
          $(this).not(':has(ul)').addClass('file');
        });

        // Open the menu
        $('#pdi_admin_menu li.container > a, #pdi_admin_menu li.container > span').click(function(event) {
          event.preventDefault();
          if ($(this).parent().hasClass('active')) {
            $(this).parent().removeClass('active');
          }else{
            $(this).parent().addClass('active');
          };
        });
      });
    }
  };
})(jQuery);