const io = require('socket.io')(process.env.PORT || 3000);
const arrUserInfo = [];

io.on('connection', socket => {
	socket.on('NEW_USER_ASSIGN',username =>{
		//check dupplicate username
		const isExist = arrUserInfo.some(e => e.name === username.name);
		socket.peerId = username.peerId;
		if (isExist) {
			return socket.emit('ASSIGN_FAIL');
		}
		arrUserInfo.push(username);
		socket.emit('ONLINE_USER_LIST',arrUserInfo);
		socket.broadcast.emit('NEW_USER',username);
	});
	socket.on('disconnect',() => {
		const index = arrUserInfo.findIndex(user => user.peerId ===socket.peerId);
		arrUserInfo.splice(index,1);
		io.emit('USER_DISCONNECT',socket.peerId);
	});
});

