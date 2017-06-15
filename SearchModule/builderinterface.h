#ifndef BUILDERINTERFACE_H
#define BUILDERINTERFACE_H

class CrawlerInteface;
class SearchModule;

class BuilderInterface
{
public:
    virtual ~BuilderInterface(){}
    virtual CrawlerInteface* build() = 0;
    virtual void setHandler(SearchModule *handler) = 0;
    virtual void initQueue() = 0;
};

#endif // BUILDERINTERFACE_H
