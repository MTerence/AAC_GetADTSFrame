//
//  ViewController.m
//  AAC_GetADTSFrame
//
//  Created by Ternence on 2019/3/25.
//  Copyright © 2019 Ternence. All rights reserved.
//

#import "ViewController.h"
#import "getADTSFrame.cpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    simplest_aac_parser("nocturne.aac");
}


@end
