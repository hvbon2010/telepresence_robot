class GETKEYBOARD{

  constructor() {
    this.name = 'Something Good';
  }

  register() {
    var that = this;
    window.addEventListener('keydown', function(e) {return that.Getkeycode(e);});
  }

  Getkeycode(e) {
    console.log(this.name);
    switch(e.keyCode) {
      case 'w':
        // some code here...
        console.log('keydown w' + e.keyCode);
        break;
      case 97:
        // some code here...
        console.log('keydown a'+e.keyCode);
        break;
        default:
        console.log('default '+e.keyCode);
    }
  }

}

var myObject = new GETKEYBOARD();
myObject.register();