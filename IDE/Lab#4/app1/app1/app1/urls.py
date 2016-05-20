"""app1 URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.9/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url, include, patterns
from django.contrib import admin
from django.conf import settings
admin.autodiscover()

urlpatterns = [
    url(r'^admin/', include(admin.site.urls)),
    url(r'^articles/', include('article.urls')),
    url(r'^accounts/', include('userprofile.urls')),
    url(r'^$', 'app1.views.home'),
    url(r'^home/$', 'app1.views.home'),
    url(r'^accounts/login/$', 'app1.views.login'),
    url(r'^accounts/auth/$', 'app1.views.auth_view'),
    url(r'^accounts/logout/$', 'app1.views.logout'),
    url(r'^accounts/loggedin/$', 'app1.views.loggedin'),
    url(r'^accounts/invalid/$', 'app1.views.invalid_login'),
    url(r'^accounts/register/$', 'app1.views.register_user'),
    url(r'^accounts/register_success/$', 'app1.views.register_success'),
]

if settings.DEBUG:
    # static files (images, css, javascript, etc.)
    urlpatterns += patterns('',
        (r'^media/(?P<path>.*)$', 'django.views.static.serve', {
        'document_root': settings.MEDIA_ROOT}))