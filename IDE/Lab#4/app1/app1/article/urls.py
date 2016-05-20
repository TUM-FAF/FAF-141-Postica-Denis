from django.conf.urls import url, include, patterns


urlpatterns = patterns('',
	url(r'^all/$','article.views.articles'),
	url(r'^get/(?P<article_id>\d+)/$','article.views.article'),
	url(r'create/$', 'article.views.create'),
	url(r'^like/(?P<article_id>\d+)/$','article.views.like_article'),
	url(r'search/$', 'article.views.search_titles'),
)

