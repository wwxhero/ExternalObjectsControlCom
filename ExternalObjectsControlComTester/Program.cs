using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using ExternalObjectsControlComLib;
namespace ExternalObjectsControlComTester
{
    class Program
    {
        static void Main(string[] args)
        {
            IDistriObjsCtrl pCtrl = new DistriObjsCtrlClass();
            pCtrl.CreateNetworkExternalObjectControl(0, 0);
        }
    }
}
