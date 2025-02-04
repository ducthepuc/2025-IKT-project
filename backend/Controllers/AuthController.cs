using Ecommerence.Core.Models;
using Ecommerence.Core.Repositories;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Configuration;
using System;
using System.Collections.Concurrent;

namespace EcommerenceApi.Controllers
{
    [Route("api/auth")]
    [ApiController]
    public class AuthController : ControllerBase
    {
        private readonly UserRepository _userRepository;
        private static ConcurrentDictionary<string, int> _sessionTokens = new();
        private static int CurrentId = 2; //2 because test user already exists

        public AuthController(UserRepository userRepository)
        {
            _userRepository = userRepository;
        }

        [HttpPost("login")]
        public IActionResult Login([FromBody] LoginDto model)
        {
            var user = _userRepository.GetUserByUsername(model.UserName);
            if (user == null || user.Password != model.Password)
            {
                return Unauthorized(new { message = "Invalid credentials" });
            }

            var sessionToken = Guid.NewGuid().ToString();
            _sessionTokens[sessionToken] = user.Id;

            return Ok(new { Token = sessionToken });
        }

        private bool ValidateSessionToken(string token)
        {
            return _sessionTokens.ContainsKey(token);
        }

        [HttpGet("profile")]
        public IActionResult GetProfile([FromHeader] string Authorization)
        {
            // Validate session token
            if (string.IsNullOrEmpty(Authorization) || !ValidateSessionToken(Authorization))
            {
                return Unauthorized(new { message = "Invalid or expired session token" });
            }

            // Get user data by token
            var userId = _sessionTokens[Authorization];
            var user = _userRepository.GetUserById(userId);

            // Return user profile info
            return Ok(new { UserName = user.UserName, Email = user.Email });
        }

        [HttpPost("register")]
        public IActionResult Register([FromBody] RegisterDto model)
        {
            if (_userRepository.GetUserByEmail(model.Email) != null || _userRepository.GetUserByUsername(model.UserName) != null)
            {
                return BadRequest(new { message = "Username or email already taken" });
            }

            // Create new user
            var newUser = new User
            {
                UserName = model.UserName,
                Email = model.Email,
                Password = model.Password,
                Id = CurrentId
            };

            CurrentId += 1;

            _userRepository.AddUser(newUser);
            return Ok(new { message = "Successful registration!" });
        }

        [HttpPost("logout")]
        public IActionResult Logout([FromHeader] string Authorization)
        {
            // Validate session token
            if (string.IsNullOrEmpty(Authorization) || !_sessionTokens.ContainsKey(Authorization))
            {
                return Unauthorized(new { message = "Invalid session token" });
            }

            // Remove session token
            _sessionTokens.TryRemove(Authorization, out _);

            // Return success message
            return Ok(new { message = "Logged out successfully!" });
        }

        [HttpDelete("{id}")]
        public IActionResult DeleteUser(int id)
        {
            if (!_userRepository.DeleteUser(id))
                return NotFound(new { message = "User not found" });

            return Ok(new { message = "User deleted successfully" });
        }
    }
}
