//
//  AppDelegate.swift
//  FBI-Like App
//
//  Created by Mr. ST4N on 15/08/14.
//  Copyright (c) 2014 Mr. ST4N. All rights reserved.
//
//  What is big, yellow and can't swim ? A bus full of children.

import Cocoa

class AppDelegate: NSObject, NSApplicationDelegate
{
                            
    @IBOutlet weak var window: NSWindow!
    @IBOutlet weak var displayImagesView: NSImageView!
    @IBOutlet weak var fbiImage: NSImageView!
    @IBOutlet weak var consoleView: NSTextField!
    @IBOutlet weak var secondConsole: NSTextField!
    @IBOutlet weak var progressBar: NSLevelIndicator!
    @IBOutlet weak var hackButton: NSButton!
    @IBOutlet weak var userLabel: NSTextField!
    @IBOutlet weak var passLabel: NSTextField!
    @IBOutlet weak var password: NSSecureTextField!
    @IBOutlet weak var username: NSTextField!
    @IBOutlet weak var accessLabel: NSTextField!
    @IBOutlet weak var about: NSButton!
    @IBOutlet weak var aboutMenu: NSMenuItem!
    var imageArray:[NSImage] = [NSImage]()
    var timer:Int = 0
    var timer2:Int = 0
    
    
    
    
    @IBAction func aboutButton(sender: AnyObject)
    {
        var mySound = NSSound(named:"Submarine.aiff")
        mySound.play()
        let myAlert:NSAlert = NSAlert()
        myAlert.messageText = "FBI: DataBase - Mr.ST4N"
        myAlert.informativeText = "Developed in Swift. For entertainment only."
        
        myAlert.runModal()
    }

    @IBAction func userValidation(sender: AnyObject)
    {
        if (username.stringValue == "user" && password.stringValue == "passwd") || (username.stringValue == "username" && password.stringValue == "passwd")
        {
            accessLabel.textColor = NSColor.greenColor()
            accessLabel.stringValue = "Acces Granted"
            accessLabel.hidden = false
            
            loadDataBase()
            
        }
        else
        {
            accessLabel.textColor = NSColor.redColor()
            accessLabel.stringValue = "Acces Denied"
            accessLabel.hidden = false
            
        }
    }

    func applicationDidFinishLaunching(aNotification: NSNotification?)
    {
        displayImagesView.hidden = true
        consoleView.hidden = true
        secondConsole.hidden = true
        progressBar.hidden = true
        hackButton.hidden = true
        about.hidden = true
        
        var alpha:CGFloat = 0.9
        consoleView.alphaValue = alpha
        secondConsole.alphaValue = alpha
        username.becomeFirstResponder()
        
    }
    @IBAction func hackFunc(sender: AnyObject)
    {
            timer++;
            timer2++;
        
            if timer == 2
            {
                if progressBar.doubleValue < 1.0
                {
                    timer = 0;
                    var rnd = Int(arc4random_uniform(23))
                    displayImagesView.image = imageArray[rnd]
                    progressBar.doubleValue += 0.01
                    var tmp = consoleView.stringValue.utf16Count
                    
                    if tmp >= 1800
                    {
                        consoleView.stringValue = "";
                    }
                    consoleView.stringValue = "\(consoleView.stringValue) \(rnd) C:\\Users\\Mr.ST4N\\\(rnd+2)Dropbox\\Governement\\Hacking\\\(rnd-3)\\Reader\\ \n"
                    
                    secondConsole.stringValue = "Data Initialized \n\n Transfer Progression: \(Int(progressBar.doubleValue * 100))% - #\(rnd)";
                    
                    if timer2 > 10 && timer < 480
                    {
                        secondConsole.stringValue = secondConsole.stringValue + "\nDownloading..."
                    }
                    if timer2 > 30
                    {
                        timer2 = 0;
                    }
                    update()
                    hackFunc(sender)
                }
                else
                {
                    update()
                    displayImagesView.image = NSImage(named: "24")
                    secondConsole.stringValue = secondConsole.stringValue + ": Transfer Completed";
                }
            }
            else
            {
                update()
                hackFunc(sender)
        }
    }

    func applicationWillTerminate(aNotification: NSNotification?) {
        // Insert code here to tear down your application
    }

    func update()
    {
        displayImagesView.display()
        consoleView.display()
        secondConsole.display()
        progressBar.display()
    }
    
    func loadDataBase()
    {
        for i in 1...23
        {
            imageArray.append(NSImage(named: "\(i)"))
        }
        
        displayImagesView.hidden = false
        consoleView.hidden = false
        secondConsole.hidden = false
        progressBar.hidden = false
        hackButton.hidden = false
        about.hidden = false
        displayImagesView.imageScaling = NSImageScaling.ImageScaleAxesIndependently
        
    
        userLabel.hidden = true
        passLabel.hidden = true
        password.hidden = true
        username.hidden = true
        accessLabel.hidden = true
    }
    

}