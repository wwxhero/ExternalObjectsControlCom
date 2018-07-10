using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ExternalObjectsControlComLib;
using System.Diagnostics;

namespace ExternalObjectsControlComTester
{
    class Program
    {
        static string s_sceneTest = "E:\\Distri_NadsMiniSim_2.2.1\\data\\distri_demo_fhwa_unitydev.scn";
        static void testCreateRelease()
        {
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl(1, 0);
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void testInitializeUnInitialize()
        {
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl(1, 0);
                pCtrl.Initialize(s_sceneTest);
                pCtrl.UnInitialize();
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void testInitializeUnInitialize2()
        {
            try
            {
                IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
                pCtrl.CreateNetworkExternalObjectControl(1, 0);
                pCtrl.Initialize(s_sceneTest);

                bool recieving = true;
                EVT evt = EVT.evtUndefined;
                string[] evtNames = { "create dynobj", "delete dynobj", "undefined" };
                while (recieving)
                {
                    bool empty = true;
                    while (true)
                    {
                        pCtrl.QFrontEvent(out evt, out empty);
                        if (empty)
                            break;
                        Console.WriteLine(evtNames[(int)evt]);
                        switch (evt)
                        {
                            case EVT.crtDyno:
                                {
                                    int id;
                                    string name;
                                    int solId;
                                    double xSize, ySize, zSize;
                                    double xPos, yPos, zPos;
                                    double xTan, yTan, zTan;
                                    double xLat, yLat, zLat;
                                    pCtrl.GetcrtDynoTuple(out id, out name, out solId
                                            , out xSize, out ySize, out zSize
                                            , out xPos, out yPos, out zPos
                                            , out xTan, out yTan, out zTan
                                            , out xLat, out yLat, out zLat);
                                    string strTuple = string.Format(@"\n\tid:{14}
                                                                  \n\tname:{0}\tsolId:{1}
                                                                  \n\tsize:<{2},{3},{4}>
                                                                  \n\tpos:<{5},{6},{7}>
                                                                  \n\ttan:<{8},{9},{10}>
                                                                  \n\tlat:<{11},{12},{13}>"
                                                                    , name, solId
                                                                    , xSize, ySize, zSize
                                                                    , xPos, yPos, zPos
                                                                    , xTan, yTan, zTan
                                                                    , xLat, yLat, zLat
                                                                    , id);
                                    Console.Write(strTuple);
                                    break;
                                }

                            case EVT.delDyno:
                                {
                                    int id;
                                    pCtrl.GetdelDynoTuple(out id);
                                    string strTuple = string.Format("\n\tid:{0}", id);
                                    Console.Write(strTuple);
                                    break;
                                }
                        }
                        pCtrl.QPopEvent();
                    }


                    ConsoleKeyInfo key = new ConsoleKeyInfo();
                    if (Console.KeyAvailable)
                        key = Console.ReadKey(true);

                    switch (key.Key)
                    {
                        case ConsoleKey.Q:
                        case ConsoleKey.Escape:
                            recieving = false;
                            break;
                    }
                }
                pCtrl.UnInitialize();
                pCtrl.ReleaseNetworkExternalObjectControl();
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }

        static void Main(string[] args)
        {
            testCreateRelease();
            testInitializeUnInitialize();
            //testInitializeUnInitialize2();
        }
    }
}
