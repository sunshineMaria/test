/*common*/
/**
 * author :  a2014.
 * date   :  15/3/10.
 */

var $ = require('jquery');
var shareTmp = require('wap/component/shareTmp')

function slider(opt) {
    this.imgs = opt.imgs;
    this.container = opt.container;
    this.can = true;
    this.init();
}

slider.prototype = {
    init: function () {
        this.el = $('<div class="slider-c"><div class="slider-inner"></div></div>', {
            css: {
                width: '100%',
                height: '80px'
            }
        })

        this.inner = this.el.find('.slider-inner');
        this.addElement();
        this.container.append(this.el);
        this.addEvent();
        this.sub();
    },
    addElement: function () {
        var me = this;
        var html = '';
        $.each(this.imgs, function (index, item) {
            html += '<div class="slider-item"><img src="' + item + '"/></div>';
        })

        var itemW = 200;
        var left = 4,
            count = this.imgs.length;
        this.inner.width((itemW + left) * count - left + 'px');
        this.inner.html(html);
    },
    addEvent: function () {
        var me = this;
        var startX = 0;
        var left = 0;
        var w = $(window).width();
        this.el.on('touchstart', function (e) {
            startX = e.originalEvent.touches[0].pageX;
            left = parseInt(me.inner.css('left'));
            me.can = 1;
        })
        this.el.on('touchmove', function (e) {

            if (!me.can)
                return;

            var p = left + e.originalEvent.touches[0].pageX - startX;
            if (p > 0) {//右拉出边界
                me.inner.trigger('slowdown', p);
            } else if (-parseInt(me.inner.css('left')) > me.inner.width() - w) {
                //左拉出边界
                me.inner.trigger('slowdown', e.originalEvent.touches[0].pageX - startX);
            }
            else {
                me.inner.css('left', p + 'px');
            }
        })
        this.el.on('touchend', function (e) {

        })
    },
    sub: function () {
        var inner = this.inner;
        var w = $(window).width();
        var me = this;
        var resistance = 1;

        this.inner.on('slowdown', function (e, p) {
            if (p > 0) {//右拉
                inner.css({'left': 0});
            } else {
                inner.css({'left': -(inner.width() - w)});
            }
            me.can = 0;
        })
    }
}

return slider;