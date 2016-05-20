from django.db import models
from django.contrib.auth.models import User

class UserProfile(models.Model):
	user = models.OneToOneField(User)
	likes_programming = models.BooleanField(default=False)
	favourite_season = models.CharField(max_length=50)

User.profile = property(lambda u: UserProfile.objects.get_or_create(user=u)[0] )
