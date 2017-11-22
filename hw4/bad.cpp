void removeBad(list<Movie*>& li)
{
	//go through list
	for (list<Movie*>::iterator it = li.begin(); it != li.end();) {
		if ((*it)->rating()<50) {//if less than 50
			Movie *movie = *it;
			it = li.erase(it);
			delete movie;//delete the removed movie
		}
		else {
			++it;//next
		}
	}
}