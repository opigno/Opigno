(function($) {

    Drupal.behaviors.opignoogaccess = {
        attach: function (context, settings) {
            var $checkboxes=$('[name="requires_validation[und]"],[name="anomymous_visibility[und]"]',context);
            if ($checkboxes.length)
            {
                var group_access=$('[name="group_access[und]"]',context);
                group_access.change(function(){
                    var value=$('[name="group_access[und]"]:checked',context).val();
                    if (value==1)
                    {
                        $checkboxes.parent().show();
                    }
                    else
                    {
                        $checkboxes.parent().hide();
                    }
                }).change()
            }
        }
    };

})(jQuery);