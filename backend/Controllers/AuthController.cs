using Ecommerence.Core.Models;
using Ecommerce.Core.Helpers;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using System.Security.Claims;
using Microsoft.IdentityModel.Tokens;
using System.Text;
using System.IdentityModel.Tokens.Jwt;
using Microsoft.AspNetCore.Cryptography.KeyDerivation;

namespace EcommerenceApi.Controllers
{
    [Route("api/auth")]
    [ApiController]
    public class AuthController : ControllerBase
    {
        private readonly ApplicationDbContext _context;
        private readonly IConfiguration _configuration;

        public AuthController(ApplicationDbContext context, IConfiguration configuration)
        {
            _context = context;
            _configuration = configuration;
        }

        // Login user and return JWT Token
        [HttpPost("login")]
        public async Task<IActionResult> Login([FromBody] LoginDto model)
        {
            var user = await _context.Users.FirstOrDefaultAsync(u => u.UserName == model.UserName);

            // Validate user credentials (using hashed password comparison)
            if (user == null || !PasswordHelper.VerifyPassword(user.Password, model.Password))
            {
                return Unauthorized(new { message = "Invalid credentials" });
            }

            // Create JWT Token
            var token = GenerateJwtToken(user);

            return Ok(new { Token = token });
        }

        // Register a new user
        [HttpPost("register")]
        public async Task<IActionResult> Register([FromBody] RegisterDto model)
        {
            var existingUser = await _context.Users.FirstOrDefaultAsync(u => u.Email == model.Email || u.UserName == model.UserName);

            if (existingUser != null)
            {
                return BadRequest(new { message = "User already exists" });
            }

            // Hash password before saving
            var hashedPassword = PasswordHelper.HashPassword(model.Password);

            var newUser = new User
            {
                UserName = model.UserName,
                Email = model.Email,
                Password = hashedPassword,
            };

            _context.Users.Add(newUser);
            await _context.SaveChangesAsync();

            return Ok(new { message = "Registration successful!" });
        }

        // Logout (No session state is required for JWT)
        [HttpPost("logout")]
        public IActionResult Logout()
        {
            // No need to manage session tokens as JWT is stateless
            return Ok(new { message = "Logged out successfully!" });
        }

        // Delete user by ID or all users if ID == -1
        [HttpDelete("delete/{id}")]
        public async Task<IActionResult> DeleteUser(int id)
        {
            if (id == -1)
            {
                var users = await _context.Users.ToListAsync();
                _context.Users.RemoveRange(users);
                await _context.SaveChangesAsync();
                return Ok(new { message = "All users deleted successfully" });
            }

            var user = await _context.Users.FindAsync(id);
            if (user == null)
            {
                return NotFound(new { message = "User not found" });
            }

            _context.Users.Remove(user);
            await _context.SaveChangesAsync();

            return Ok(new { message = "User deleted successfully" });
        }

        // Get all users (just for testing purposes, can be restricted in production)
        [HttpGet("GetAll")]
        public async Task<IActionResult> GetAllUsers()
        {
            var users = await _context.Users.ToListAsync();
            return Ok(users);
        }

        // Get user profile from JWT Token
        [HttpGet("profile")]
        public async Task<IActionResult> GetProfile([FromHeader] string Authorization)
        {
            if (string.IsNullOrEmpty(Authorization))
            {
                return Unauthorized(new { message = "Authorization header is missing" });
            }

            var userId = GetUserIdFromToken(Authorization);

            var user = await _context.Users.FindAsync(userId);
            if (user == null)
            {
                return Unauthorized(new { message = "User not found" });
            }

            return Ok(new { UserName = user.UserName, Email = user.Email });
        }

        // Decode JWT and get User ID
        private int GetUserIdFromToken(string token)
        {
            var tokenHandler = new JwtSecurityTokenHandler();
            var jwtToken = tokenHandler.ReadJwtToken(token);
            var userIdClaim = jwtToken.Claims.FirstOrDefault(c => c.Type == ClaimTypes.NameIdentifier);
            return userIdClaim != null ? int.Parse(userIdClaim.Value) : 0;
        }

        // Generate JWT Token
        private string GenerateJwtToken(User user)
        {
            var claims = new[]
            {
                new Claim(ClaimTypes.NameIdentifier, user.Id.ToString()),
                new Claim(ClaimTypes.Name, user.UserName),
                new Claim(ClaimTypes.Email, user.Email),
            };

            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(_configuration["JwtSettings:SecretKey"]));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

            var expirationTime = DateTime.UtcNow.AddHours(1);

            var token = new JwtSecurityToken(
                issuer: _configuration["JwtSettings:Issuer"],
                audience: _configuration["JwtSettings:Audience"],
                claims: claims,
                expires: expirationTime,
                signingCredentials: creds
            );

            return new JwtSecurityTokenHandler().WriteToken(token);
        }
    }
}
