from django.conf.urls import url, include, patterns

urlpatterns = patterns('',
		url(r'profile/$', 'userprofile.views.user_profile'),
)