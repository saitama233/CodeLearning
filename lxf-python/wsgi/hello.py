def application(environ, start_response):
    status = '200 OK'
    headers = [('Content-Type', 'text/html')]
    start_response(status, headers)
    html = '<h1>Hello,%s' %(environ['PATH_INFO'][1:] or 'wsgi')
    return html
