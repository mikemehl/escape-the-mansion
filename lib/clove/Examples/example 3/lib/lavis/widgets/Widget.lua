--[[
	Every widget share some properties like position, size and some events.
	So every widget rather then redefining these common properties, will
	simply inherit from the Widget class.
]]

lavis.widget=lavis.Class{
	name=nil,                  --a property you could use to identify the widget
	visible=true,              --is the widget visible?
	enabled=true,              --whether you can interact with the widget
	value=nil,                 --extra information for each widget
	image=nil,                 --assumption: most widgets *will* have an image
	entered=false,             --state of the widget (hovered or not)
	focused=false,             --if you click on a widget once it's focused
	x=0,                       --x and y for position of the widget
	y=0,
	imgX=0,                    --imgX and imgY for position of the image
	imgY=0,
	r=0,                       --r for rotating the image
	sx=1,                      --sx,sy for scaling the image
	sy=1,
	ox=0,                      --ox,oy for setting origin of the image
	oy=0,
	drawColor=nil,             --an array in {r,g,b,a} format, sets color before drawing
	drawBorder=nil,            --if true draws a simple border around the widget (UFD)
	drawOrigin=nil,            --if true draws the origin of the widget and image(UFD)
	shape='box',               --useful for detecting hover (box,circle,ellipse)
	width=nil,                 --width and height only useful for box shape
	height=nil,
	radiusa=nil,               --for circle and ellipse
	radiusb=nil,               --for ellipse only
	responsive=false,           --whether changing widget's size affects image's size
	frozen=nil,                --if frozen is true then *widget* won't move or scale
	escapekeys=nil,            --the keys which on pressing makes the widget lose focus
	hotbtns=nil,            --the mouse btns which on pressing makes the widget gain focus

	onRelease=function() end,      --when a MB is released over the widget
	onClick=function() end,        --when a MB is clicked over the widget
	onResize=function() end,       --when the widget is resized!
	onMove=function() end,         --when the widget is moved!
	onKeyPress=function() end,     --when a certain key is pressed *while focused*
	onFocusGained=function() end,  --called when the widget is focused
	onFocusLost=function() end,    --called when the widget has lost focus
	whileFocused=function() end,   --called as long as the widget is focused
	whileKeyPressed=function() end,--*as long as* a key is pressed *while* focused
	whilePressed=function() end,   --is called *while* LMB is down
	whileHovered=function() end,   --is called *while* the cursor is over the widget
	onMouseMove=function() end,    --is called when the cursor moves while still hovered
	onWheelMove=function() end,    --is called when scrolled while the widget is still focused
	onMouseEnter=function() end,        --when cursor enters the widget area
	onMouseExit=function() end,         --when cursor exits the widget area
}

local r,g,b,a  -- to get the default color

function lavis.widget:init(...) self:initWidget(...) end

function lavis.widget:initWidget(x,y,shape,...)
	self.escapekeys,self.hotbtns={},{true}
	assert(type(shape)=='string',
		"Lavis Error! Expected a string for shape, got '"..shape.."' !"
	)
	self:setShape(shape,...)
	table.insert(lavis.widgets,self)
	self.responsive=true
	self:setPosition(x,y)
end

function lavis.widget:setShape(shape,...)
	self.shape=shape
	self:setSize(...)
end

function lavis.widget:setPosition(x,y,ignore)
	if self.frozen then return end
	self.x=x or self.x
	self.y=y or self.y
	if self.responsive and not ignore then self:setImagePosition(x,y) end
	self:onMove(x,y,ignore)
end

function lavis.widget:getSize()
	if self.shape=='box' then return self.width,self.height
	elseif self.shape=='circle' then return self.radiusa
	elseif self.shape=='ellipse' then return self.radiusa,self.radiusb end
end


function lavis.widget:setSize(arg1,arg2,arg3)

	if self.frozen then return end
	
	local origw,origh=self.width,self.height

	if self.shape=='box' then
		if not self.width then
			self.width,self.height=arg1,arg2
			return
		end		
		--arg1,arg2 are w,h and arg3 is relative

		-- useful information for translating the widget responsively
		local dx,dy=self.width,self.height

		--in case the box is a square
		if type(arg2)=='boolean' then arg3=arg2 end
		arg2=type(arg2)=='number' and arg2 or arg1

		if arg3==true then
			dx,dy=arg1,arg2
			arg1,arg2=self.width+arg1,self.height+arg2
		else
			dx=arg1-self.width
			dy=arg2-self.height
		end

		self.width,self.height=arg1,arg2
		
		if self.responsive and self.image then
			self.x=self.x-dx/(self.image:getWidth()/self.ox)
			self.y=self.y-dy/(self.image:getHeight()/self.oy)
			self:setImageSize(arg1,arg2,false)
		end

	elseif self.shape=='circle' then
		--arg1,arg2 are radius,relative and arg3 is nil
		if arg2==true then
			self.radiusa=(self.radiusa or 0)+arg1
		else
			self.radiusa=arg1
		end
		if self.responsive then
			self:setImageSize(arg1*2,arg1*2,arg2)
		end
	elseif self.shape=='ellipse' then
		assert(arg1 and arg2,"lavis Error! You must provide both the radiuses!")
		if arg3 then arg1,arg2=self.radiusa+arg1,self.radiusb+arg2 end
		self.radiusa,self.radiusb=arg1,arg2
		if self.responsive then
			self:setImageSize(arg1*2,arg2*2,false)
		end
	else
		error("lavis Error! The given string is not a valid shape!")
	end
	self.onResize(arg1,arg2,arg3)
end

function lavis.widget:getColor() return self.drawColor end
function lavis.widget:setColor(...) self.drawColor={...} end

function lavis.widget:setImage(url)
	if type(url)~='string' then
		self.image=url
	else
		self.image=love.graphics.newImage(url)
	end
end

function lavis.widget:setImagePosition(x,y)
	self.imgX = (x or self.imgX) + (self.shape=='box' and self.ox or 0)
	self.imgY = (y or self.imgY) + (self.shape=='box' and self.oy or 0)
end

function lavis.widget:getImagePosition()
	return self.imgX,self.imgY
end

function lavis.widget:getImageWidth()
	return self.image and self.sx*self.image:getWidth() or 0
end
function lavis.widget:getImageHeight()
	return self.image and self.sy*self.image:getHeight() or 0
end
function lavis.widget:getImageSize()
	return self:getImageWidth(),self:getImageHeight()
end


function lavis.widget:setImageSize(w,h,relative)

	if not self.image then return end
	
	if type(h)=='boolean' or not h then h,relative=w,h end	--for square
	if relative then w,h=w+self:getImageWidth(),h+self:getImageHeight() end

	self.sx,self.sy=w/self.image:getWidth(),h/self.image:getHeight()
end

function lavis.widget:setImageRotation(r) self.r=r end
function lavis.widget:setImageOrigin(...)
	self.ox,self.oy=...
	self:setImagePosition()
end

-- function lavis.widget:showOrigin() self.drawOrigin=true end
-- function lavis.widget:hideOrigin() self.drawOrigin=nil end

-- function lavis.widget:showBorder() self.drawBorder=true end
-- function lavis.widget:hideBorder() self.drawBorder=nil end


function lavis.widget:isFrozen() return self.frozen end
-- function lavis.widget:freeze() self.frozen=true end
-- function lavis.widget:unfreeze() self.frozen=nil end

function lavis.widget:setFrozen(val)  self.frozen=val end
function lavis.widget:setVisible(val) self.visible=val end
function lavis.widget:toggleVisibilty() self.visible=not self.visible end
function lavis.widget:setEnabled(val) self.enabled=val end
function lavis.widget:setName(val) self.name=val end
function lavis.widget:setValue(val) self.value=val end
function lavis.widget:setWireframe(val) self.drawBorder,self.drawOrigin=val,val end
function lavis.widget:setResponsive(val) self.responsive=val end
function lavis.widget:isResponsive(val) return self.responsive end

function lavis.widget:setValue(val) self.value=val end
function lavis.widget:getValue() return self.value end

function lavis.widget:setFocus(val,...)
	if val==true then
		if not self.focused then
			self.focused=true
			self.onFocusGained(...)
		end
	else
		if self.focused then
			self.focused=false
			self.onFocusLost(...)
		end
	end
end

function lavis.widget:isEnabled() return self.enabled end
-- function lavis.widget:enable() self.enabled=true end
-- function lavis.widget:disable() self.enabled=false end

function lavis.widget:isVisible() return self.visible end
-- function lavis.widget:show() self.visible=true end
-- function lavis.widget:hide() self.visible=false end


function lavis.widget:updateWidget(dt)
	if self.focused then self.whileFocused() end
	if self:isHovered() then
		self.whileHovered()
		if not self.entered then
			self.entered=true
			self.onMouseEnter()
		end
		if love.mouse.isDown(1) then
			self.whilePressed()
		end
	else
		if self.entered then
			self.entered=false
			self.onMouseExit()
		end
	end
end

function lavis.widget:update(dt)
	self:updateWidget(dt)
end

function lavis.widget:keypressed(key,...)
	if self.focused then self.onKeyPress(key,...) end
	if self.escapekeys[key] then
		self:setFocus(false,"keypressed",key,...)
	end
end

function lavis.widget:mousepressed(x,y,button,...)
	
	if self:isHovered(x,y) then
		if self.hotbtns[button] then
			self:setFocus(true,"mousepressed",x,y,...)
		end
		self.onClick(button,x,y,...)
	else
		self:setFocus(false,"mousepressed",button,x,y,...)
	end
end

function lavis.widget:mousemoved(x,y,...)
	if self:isHovered(x,y) then
		self.onMouseMove(x,y,...)
	end
end

function lavis.widget:wheelmoved(x,y)
	if self.focused then
		self.onWheelMove(x,y)
	end
end

function lavis.widget:mousereleased(x,y,button,...)
	if self.focused then
		self.onRelease(button,x,y,...)
	end
end

function lavis.widget:isHovered(mx,my)
	if not mx then mx,my=lavis.getCursorPosition() end

	if self.shape=='box' then
		return lavis.aabb(self.x,self.y,self.width,self.height,mx,my)

	elseif self.shape=='circle' then
		return lavis.pie(self.x,self.y,self.radiusa,self.radiusa,mx,my)

	elseif self.shape=='ellipse' then
		return lavis.pie(self.x,self.y,self.radiusa,self.radiusb,mx,my)
	end
end

function lavis.widget:isFocused() return self.focused end

function lavis.widget:connectSignalWidget(signal,handler)
	if signal=="click" or "hit" then
		self.onClick=handler
	elseif signal=="release" then
		self.onRelease=handler
	elseif signal=="hovered" then
		self.whileHovered=handler
	elseif signal=="pressed" or signal=="clicked" then
		self.whilePressed=handler
	elseif signal=="mousemoved" then
		self.onMouseMove=handler
	elseif signal=="wheelmoved" or signal=="scroll" then
		self.onWheelMove=handler
	elseif signal=="focused" then
		self.whileFocused=handler
	elseif signal=="focus_gained" or signal=="focus" then
		self.onFocusGained=handler
	elseif signal=="focus_lost" or signal=="blur" then
		self.onFocusLost=handler
	elseif signal=="enter" or signal=="mouseenter" then
		self.onMouseEnter=handler
	elseif signal=="exit" or signal=="mouseexit" then
		self.onMouseExit=handler
	elseif signal=="move" then
		self.onMove=handler
	elseif signal=="resize" then
		self.onResize=handler
	end
end

function lavis.widget:addEscapeKeys(...)
	local tbl=type((...))=='table' and (...) or {...}
	for i=1,#tbl do
		self.escapekeys[tbl[i]]=true
	end
end

function lavis.widget:setEscapeKey(key,remove)
	self.escapekeys[key]=not remove
end

function lavis.widget:setHotButton(btn,remove)
	self.hotbtns[btn]=not remove
end


function lavis.widget:connectSignal(...)
	self:connectSignalWidget(...)
end

lavis.widget.addEventListener=lavis.widget.connectSignal

function lavis.widget:render()
	if self.drawColor then
		r,g,b,a=love.graphics.getColor()
		love.graphics.setColor(unpack(self.drawColor))
	end

	love.graphics.draw(self.image,self.imgX,self.imgY,
		self.r,self.sx,self.sy,self.ox,self.oy)
	
	if self.drawBorder then self:renderBorder() end
	if self.drawOrigin then self:renderOrigin() end
	
	if self.drawColor then love.graphics.setColor(r,g,b,a) end
end

local function renderArrow(x,y,dir,a,b,c)
	
	a,b,c=a or 20,b or 5,c or 30

	if dir=='right' or dir=='left' then
		if dir=='left' then a,c=-a,-c end
		love.graphics.line(x,y,x+a,y)
		love.graphics.polygon('fill',
			x+a,y-b,
			x+a,y+b,
			x+c,y
		)
	elseif dir=='down' or dir=='up' then
		if dir=='up' then a,c=-a,-c end
		love.graphics.line(x,y,x,y+a)
		love.graphics.polygon('fill',
			x-b,y+a,
			x+b,y+a,
			x,y+c
		)
	end
end

function lavis.widget:renderOrigin()

	love.graphics.setColor(1,0,0,1)
	love.graphics.circle('fill',self.x,self.y,5)
	
	renderArrow(self.x,self.y,"right")
	renderArrow(self.x,self.y,"down")
	if self.shape~='box' then
		renderArrow(self.x,self.y,"up")
		renderArrow(self.x,self.y,"left")
	end
	
	if self.image then
		love.graphics.setColor(0,1,0,0.7)
		love.graphics.circle('fill',self.imgX,self.imgY,5)
	
		if self.ox<self.image:getWidth() then
			renderArrow(self.imgX,self.imgY,"right")
		end
		if self.ox>0 then
			renderArrow(self.imgX,self.imgY,"left")
		end
		if self.oy<self.image:getHeight() then
			renderArrow(self.imgX,self.imgY,"down")
		end
		if self.oy>0 then
			renderArrow(self.imgX,self.imgY,"up")
		end
	end
	love.graphics.setColor(1,1,1)
end

function lavis.widget:renderBorder()
	love.graphics.setColor(1,0,0,1)
	if self.shape=='box' then
		love.graphics.rectangle('line',self.x,self.y,self.width,self.height)
	elseif self.shape=='circle' then
		love.graphics.circle('line',self.x,self.y,self.radiusa)
		love.graphics.rectangle('line',self.x-self.radiusa,self.y-self.radiusa,self.radiusa*2,self.radiusa*2)
	elseif self.shape=='ellipse' then
		love.graphics.rectangle('line',self.x-self.radiusa,self.y-self.radiusb,self.radiusa*2,self.radiusb*2)
		love.graphics.ellipse('line',self.x,self.y,self.radiusa,self.radiusb)
	end
	love.graphics.setColor(1,1,1)
end

