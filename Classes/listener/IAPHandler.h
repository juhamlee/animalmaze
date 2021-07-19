//
//  IAPs.hpp
//  alphabetmaze-mobile
//
//  Created by 이주함 on 11/06/2019.
//

#ifndef IAPs_hpp
#define IAPs_hpp

#include <stdio.h>
#include "PluginIAP/PluginIAP.h"

class IAPHandler : public sdkbox::IAPListener
{
private:
    virtual void onInitialized(bool ok) override;
    virtual void onSuccess(sdkbox::Product const& p) override;
    virtual void onFailure(sdkbox::Product const& p, const std::string &msg) override;
    virtual void onCanceled(sdkbox::Product const& p) override;
    virtual void onRestored(sdkbox::Product const& p) override;
    virtual void onProductRequestSuccess(std::vector<sdkbox::Product> const &products) override;
    virtual void onProductRequestFailure(const std::string &msg) override;
    void onRestoreComplete(bool ok, const std::string &msg) override;
    
public:
    static IAPHandler* getInstance();
    static IAPHandler* sharedInstance;
};

#endif /* IAPs_hpp */
