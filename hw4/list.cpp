void listAll(string path, const Domain* d) 
{
	if (d->subdomains().size() == 0)
	{
		cout << path << endl;
		return;
	}

	for (vector<Domain*>::const_iterator i = d->subdomains().begin(); i != d->subdomains().end(); i++) {

		if (path != "") {
			listAll((*i)->label() + "." + path, *i);
		}

		else {
			listAll((*i)->label() + path, *i);
		}
	}
}