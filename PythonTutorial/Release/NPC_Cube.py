class NPC_Cube:
	timesAnswered = 0

	def dialog(self):
		if self.timesAnswered == 0:
			answer = "Hi there."
		elif self.timesAnswered == 1:
			answer = "I am just a cube, lost in time and space."
		elif self.timesAnswered == 2:
			answer = "What do you want, stranger?"
		elif self.timesAnswered == 3:
			answer = "Please, tell me, what do you want, or leave me alone."
		elif self.timesAnswered == 4:
			answer = "Please, stop clicking me. Now."
		else:
			answer = "Stop! Counter: " + str(self.timesAnswered) + " times. Stop!"

		self.timesAnswered += 1
		return answer