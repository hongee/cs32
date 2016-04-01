void listAll(const Class* c, string path)  // two-parameter overload
{
    path += c->name();
    cout << path << endl;
    path += "=>";

    vector<Class*> v = c->subclasses();
    for(vector<Class*>::const_iterator it = v.begin(); it != v.end(); it++) {
      listAll(*it, path);
    }
}
