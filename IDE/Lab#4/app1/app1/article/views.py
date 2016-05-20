from django.shortcuts import render_to_response
from article.models import Article
from django.http import HttpResponse
from forms import ArticleForm
from django.http import HttpResponseRedirect
from django.core.context_processors import csrf
from django.views.decorators.csrf import csrf_protect
from django.template import RequestContext
# Create your views here.

def articles(request):
	#args = RequestContext(request)
	#args.update(csrf())
	args = {}
	args.update(csrf(request))

	args['articles'] = Article.objects.all()
	return render_to_response('articles.html', args)

def article(request, article_id=1):
	return render_to_response('article.html', {'article': Article.objects.get(id=article_id)})

def create(request):
	if request.POST:
		form=ArticleForm(request.POST, request.FILES)
		if form.is_valid():
			form.save()

			return HttpResponseRedirect('/articles/all')
	else:
		form = ArticleForm()

	args = {}
	args.update(csrf(request))

	args['form'] = form

	return render_to_response('create_article.html',args)

def like_article(request,article_id):
	if article_id:
		a = Article.objects.get(id=article_id)
		count = a.likes
		count +=1
		a.likes = count
		a.save()

		return HttpResponseRedirect('/articles/get/%s' % article_id)

def search_titles(request):
	if request.method == "POST":
		search_text = request.POST['search_text']
	else:
		search_text = ''

	articles = Article.objects.filter(title__contains = search_text)
	return render_to_response('ajax_search.html',{'articles': articles})

