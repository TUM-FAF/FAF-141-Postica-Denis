from django.db import models
from time import time

def get_upload_file_name(instance,filename):
	return "static/assets/images/%s_%s" % (str(time()).replace('.','_'),filename)

class Article(models.Model):
	title = models.TextField(max_length=200)
	body = models.TextField()
	pub_date=models.DateTimeField('date published')
	likes = models.IntegerField(default=0)
	thumbnail = models.FileField(upload_to=get_upload_file_name)

	def __unicode__(self):
		return self.title

	