//
//  main.m
//  CaoWeiTest1
//
//  Created by CaoWei on 2021/11/6.
//

#import <Foundation/Foundation.h>

@interface Clazz : NSObject

@property (nonatomic, assign) int i;

@end

@implementation Clazz

- (int)i
{
    return self->_i + 3;
}

@end


int main(int argc, const char * argv[]) {
    @autoreleasepool {
        Clazz *obj = [[Clazz alloc] init];
        obj.i = 1;
        NSLog(@"Hello, World! i is %i.", obj.i);
    }
    return 0;
}
